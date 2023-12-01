//
// Created by stefan on 10/7/23.
//

#include <algorithm>
#include "CommandProcessor.h"

namespace video::scanVideo::graphics::command {

    CommandProcessor::CommandProcessor(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
        baseColour = 0;
        lineBuffer.fill(baseColour);
        mutex_init(&isFrameComputedMutex);
    }


    //Called from CPU0
    void CommandProcessor::addCommand(std::unique_ptr<BaseCommand> baseCommand) {
        commandsToProcess.push_back(std::move(baseCommand));
        computeFrame();
    }

    //Called from CPU0
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

        mutex_enter_blocking(&isFrameComputedMutex);
        isFrameComputed = false; //Stop rendering while we're deleting the data structures.
        mutex_exit(&isFrameComputedMutex);

        commandsToProcess.clear();

        mutex_enter_blocking(&isFrameComputedMutex);
        rleRunsForLine.clear();
        lineBuffer.fill(baseColour);
        computeFrame();
        isFrameComputed = false; //Optimization to quick-skip frames until an object is added.
        mutex_exit(&isFrameComputedMutex);
    }

    //Called from CPU1
    void CommandProcessor::render_computed(scanvideo_scanline_buffer_t *scanline_buffer) {

        mutex_enter_blocking(&isFrameComputedMutex);
        bool safe_isFrameComputed = this->isFrameComputed;
        mutex_exit(&isFrameComputedMutex);

        if (!safe_isFrameComputed) {
            skipScanline(scanline_buffer);
            return;
        }
        uint16_t line_num = scanvideo_scanline_number(scanline_buffer->scanline_id);

        mutex_enter_blocking(&isFrameComputedMutex);
        if (rleRunsForLine.count(line_num) == 0) {
            //No command has artifacts for the line, so skip it
            //TODO do we want to draw the fillColour here?
            skipScanline(scanline_buffer);
        } else {
            //We have a scanline to draw
            drawScanline(scanline_buffer, rleRunsForLine[line_num]);
        }
        mutex_exit(&isFrameComputedMutex);
    }

    //Called from CPU1
    void CommandProcessor::skipScanline(scanvideo_scanline_buffer_t *scanline_buffer) {
        scanline_buffer->data_used = 0;
        scanline_buffer->status = SCANLINE_SKIPPED;
    }

    //Called from CPU1
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

    //Called from CPU0
    void CommandProcessor::computeFrame() {

        std::map<uint16_t, std::vector<RleRun>> unsortedRleRuns = std::map<uint16_t, std::vector<RleRun>>();
        for (const auto &command: commandsToProcess) {

            auto generated = command->getRleRunsForShape();
            //For every generated run,
            for (const auto g : generated) {
                if (unsortedRleRuns.count(g.first) == 0) {
                    unsortedRleRuns[g.first] = std::vector<RleRun>();
                }
                for (const auto gp : g.second) {
                    unsortedRleRuns[g.first].push_back(gp);
                }
            }
        }
        
        //Mark the frame as not computed while we are computing it. (CPU1 will daw
        mutex_enter_blocking(&isFrameComputedMutex);
        isFrameComputed = false;
        mutex_exit(&isFrameComputedMutex);

        mutex_enter_blocking(&isFrameComputedMutex);
        for (const auto &g : unsortedRleRuns) {
            std::unique_ptr<std::vector<RleRun>> runs = std::make_unique<std::vector<RleRun>>(g.second);
            rleRunsForLine[g.first] = mergeRuns(std::move(runs));
        }
        mutex_exit(&isFrameComputedMutex);

        mutex_enter_blocking(&isFrameComputedMutex);
        isFrameComputed = true;
        mutex_exit(&isFrameComputedMutex);
    }

    //Called from CPU0
    std::vector<RleRun> CommandProcessor::mergeRuns(std::unique_ptr<std::vector<RleRun>> runs) {
        std::vector<RleRun> retVec = std::vector<RleRun>();

        lineBuffer.fill(baseColour);

        //Expand each RleRun into the line buffer
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

    //Called from CPU0
    void CommandProcessor::setBaseColour(uint32_t color) {
        this->baseColour = color;
        clearFrame();
    }

    //Called from CPU0
    void CommandProcessor::clearScanlines(uint16_t min, uint16_t max) {

        mutex_enter_blocking(&isFrameComputedMutex);
        for (int i = min; i <= max; i++) {
            if (rleRunsForLine.count(i) > 0) {
                if(rleRunsForLine[i].size() > 0) {
                    rleRunsForLine[i].clear();
                }
            }
        }
        mutex_exit(&isFrameComputedMutex);

    }


} // command