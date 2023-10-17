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

    void CommandProcessor::addRleRuns(
            std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs) {

        RleRunContainerCommand containerCommand = RleRunContainerCommand(runs);

        std::unique_ptr<BaseCommand> ptr = std::make_unique<RleRunContainerCommand>(containerCommand);

        commandsToProcess.push_back(std::move(ptr));

        computeFrame();
    }

    void CommandProcessor::clearFrame() {
        //TODO need to clear the pointers in the commandsToProcess list, since we OOM.
        //TODO loop through the list and delete each pointer.

        isFrameComputed = false; //Stop rendering while we're deleting the data structures.

        //

        commandsToProcess.clear();


        rleRunsForLine.clear();
        lineBuffer.fill(baseColour);
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
            if (run.getLen() == 1) {
                *p++ = COMPOSABLE_RAW_1P;
                *p++ = run.getColour();
            }
            if (run.getLen() == 2) {
                *p++ = COMPOSABLE_RAW_2P;
                *p++ = run.getColour();
                *p++ = run.getColour();
            }
            if (run.getLen() >= 3) {
                *p++ = COMPOSABLE_COLOR_RUN;
                *p++ = run.getColour();
                *p++ = run.getLen() - 3;
            }
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
            std::unique_ptr<std::vector<RleRun>> runs = std::make_unique<std::vector<RleRun>>(g.second);
            rleRunsForLine[g.first] = mergeRuns(std::move(runs));
        }

        isFrameComputed = true;
    }

    std::vector<RleRun> CommandProcessor::mergeRuns(std::unique_ptr<std::vector<RleRun>> runs) {
        std::vector<RleRun> retVec = std::vector<RleRun>();

        lineBuffer.fill(baseColour);

        for (auto run : *runs) {
            for (int i = run.getStartX(); i <= run.getStartX() + run.getLen(); i++) {
                if (i < 0 || i >= maxDisplayWidth) {
                    continue;
                }
                lineBuffer[i] = run.getColour();
            }
        }

        //Now we compress the line buffer.
        RleRun runToInsert = RleRun(0, 1, baseColour);
        for (int i = 0; i < maxDisplayWidth - 1; i++) {

            runToInsert.setColour(lineBuffer[i]);

            if (lineBuffer[i] == lineBuffer[i + 1]) {
                runToInsert.setLen(runToInsert.getLen() + 1);
                if (i + 1 >= maxDisplayWidth) {
                    //We're at the end of the line, insert the run
                    retVec.emplace_back(runToInsert);
                }
            } else {
                //We're at the end of a run. Insert it into the vector and refresh.
                retVec.emplace_back(runToInsert);
                runToInsert = RleRun(i + 1, 1, lineBuffer[i+1]);
            }

        }
        return retVec;
    }


} // command