//
// Created by stefan on 10/7/23.
//

#include <algorithm>
#include "CommandProcessor.h"

namespace video::scanVideo::graphics::command {

    CommandProcessor::CommandProcessor(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
    }

    uint8_t CommandProcessor::getUserFrameState() {
        return userFrameState;
    }

    void CommandProcessor::setUserFrameState(uint8_t frameState) {
        this->userFrameState = frameState;
    }

    void CommandProcessor::addCommand(std::unique_ptr<BaseCommand> baseCommand) {
        commandsToProcess.push_back(std::move(baseCommand));
        computeFrame();
    }

    void CommandProcessor::clearFrame() {
        commandsToProcess.clear();
        computeFrame();
        isFrameComputed = false; //Optimization to quick-skip frames until an object is added.
    }

    void CommandProcessor::render_computed(scanvideo_scanline_buffer_t *scanline_buffer) {
        if (!isFrameComputed) {
            skipScanline(scanline_buffer);
            return;
        }
        uint16_t line_num = scanvideo_scanline_number(scanline_buffer->scanline_id);

        if (rleRunsForLine.count(line_num) == 0) {
            //No command has artifacts for the line, so skip it
            skipScanline(scanline_buffer);
            return;
        } else {
            //We have a scanline to draw
            drawScanline(scanline_buffer, rleRunsForLine[line_num]);
        }
    }

    void CommandProcessor::skipScanline(scanvideo_scanline_buffer_t *scanline_buffer) {
        scanline_buffer->data_used = 0;
        scanline_buffer->status = SCANLINE_SKIPPED;
    }

    void CommandProcessor::drawScanline(
            scanvideo_scanline_buffer_t *scanline_buffer,
            std::vector<RleRun> merged) {

        //We assume the `merged` vector is sorted and has no duplicates by
        //the time we get here.

        uint16_t *p = (uint16_t *) scanline_buffer->data;

        for (auto run : merged) {
            *p++ = COMPOSABLE_COLOR_RUN;
            *p++ = run.getColour();
            *p++ = run.getLen();
        }

        // black pixel to end line
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = 0;
        // end of line with alignment padding
        *p++ = COMPOSABLE_EOL_SKIP_ALIGN;
        *p++ = 0;

        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);

        scanline_buffer->status = SCANLINE_OK;
    }

    void CommandProcessor::computeFrame() {

        std::map<uint16_t, std::vector<RleRun>> unsortedRleRuns = std::map<uint16_t, std::vector<RleRun>>();
        for (const auto &command: commandsToProcess) {

            auto generated = command->getRleRunsForShape();

            for (const auto g : generated) {
                if (unsortedRleRuns.count(g.first) == 0) {
                    unsortedRleRuns[g.first] = std::vector<RleRun>();
                }
                for (const auto gp : g.second) {
                    unsortedRleRuns[g.first].push_back(gp);
                }
            }
        }

        for (const auto &g : unsortedRleRuns) {
            rleRunsForLine[g.first] = mergeRuns(g.second);
        }

        isFrameComputed = true;
    }

    std::vector<RleRun> CommandProcessor::mergeRuns(std::vector<RleRun> runs) {
        std::vector<RleRun> retVec = std::vector<RleRun>();

        //We only need a 400*234 display max, so we'll just use a line buffer.
        //Someday I'll clean this function up, but is should consumer only ~2kb of ram,
        //So it's already better than a frame buffer.
        const uint16_t maxDisplayWidth = 400;

        const uint32_t baseColour = 0;

        std::array<uint32_t, maxDisplayWidth> lineBuffer = {baseColour};
        struct
        {
            bool operator()(RleRun a, RleRun b) const { return a.getStartX() < b.getStartX(); }
        }
        comparator;

        std::sort(runs.begin(), runs.end(), comparator);

        for (auto run : runs) {
            for (int i = run.getStartX(); i <= run.getStartX() + run.getLen() - 1; i++) {
                if (i < 0 || i >= maxDisplayWidth) {
                    continue;
                }
                lineBuffer[i] = run.getColour();
            }
        }

        //Now we compress the line buffer.
        RleRun runToInsert = RleRun(0, 1, baseColour);
        for (int i = 0; i < maxDisplayWidth - 1; i++) {
            if (lineBuffer[i] == lineBuffer[i + 1]) {
                runToInsert.setColour(lineBuffer[i]);
                runToInsert.setLen(runToInsert.getLen() + 1);
            } else {
                //We're at the end of a run. Insert it into the vector and refresh.
                retVec.emplace_back(runToInsert);
                runToInsert = RleRun(i + 1, 1, lineBuffer[i+1]);
            }

        }
        return retVec;
    }
} // command