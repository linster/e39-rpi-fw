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
        mutex_init(&pendingMutex);
        mutex_init(&renderMutex);
    }

    //Called from either core.
    void CommandProcessor::addCommand(std::unique_ptr<BaseCommand> baseCommand) {
        //Rasterize outside any lock; this only touches the command itself.
        std::map<uint16_t, std::vector<RleRun>> shapeRuns = baseCommand->getRleRunsForShape();

        mutex_enter_blocking(&pendingMutex);
        for (auto &g : shapeRuns) {
            std::vector<RleRun> &pending = rleRunsToProcess[g.first];
            pending.insert(pending.end(), g.second.begin(), g.second.end());
        }

        if (isImmediateMode) {
            computeFrameLocked();
        }
        mutex_exit(&pendingMutex);
    }

    //Called from either core.
    void CommandProcessor::addRleRuns(
            std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs) {
        //Same path as any other command: rasterized into rleRunsToProcess in call order,
        //so drawing order is preserved. (Previously these went into commandsToProcess,
        //which computeFrame stopped reading in 123f0b3: they leaked and never rendered.)
        addCommand(std::make_unique<RleRunContainerCommand>(std::move(runs)));
    }

    void CommandProcessor::clearFrame() {
        mutex_enter_blocking(&pendingMutex);
        clearFrameLocked();
        mutex_exit(&pendingMutex);
    }

    void CommandProcessor::clearFrameLocked() {
        clearCommandsToProcess();
        lineBuffer.fill(baseColour);

        //Detach the computed frame under the render lock (O(1) swap), free it after releasing.
        std::map<uint16_t, std::vector<RleRun>> oldFrame;
        mutex_enter_blocking(&renderMutex);
        rleRunsForLine.swap(oldFrame);
        isFrameComputed = true; //An empty frame is a valid, computed frame.
        mutex_exit(&renderMutex);
        //oldFrame destroyed here, outside renderMutex.
    }

    //Caller holds pendingMutex.
    void CommandProcessor::clearCommandsToProcess() {
        rleRunsToProcess.clear();
    }

    //Called from CPU1, inside BaseScanProgram's renderCs (IRQs off): keep it short, no allocation.
    void CommandProcessor::render_computed(scanvideo_scanline_buffer_t *scanline_buffer) {
        uint16_t line_num = scanvideo_scanline_number(scanline_buffer->scanline_id);

        mutex_enter_blocking(&renderMutex);

        if (!this->isFrameComputed) {
            mutex_exit(&renderMutex);
            skipScanline(scanline_buffer);
            return;
        }

        auto line = rleRunsForLine.find(line_num);
        if (line == rleRunsForLine.end()) {
            uint32_t colour = baseColour;
            mutex_exit(&renderMutex);
            if (colour != 0) {
                drawSolidColorScanline(scanline_buffer, colour);
            } else {
                //No command has artifacts for the line, so skip it
                skipScanline(scanline_buffer);
            }
            return;
        }

        //Drawn under the lock so the vector can't be freed or swapped mid-read.
        drawScanline(scanline_buffer, line->second);
        mutex_exit(&renderMutex);
    }

    //Called from CPU1
    void CommandProcessor::skipScanline(scanvideo_scanline_buffer_t *scanline_buffer) {
        scanline_buffer->data_used = 0;
        scanline_buffer->status = SCANLINE_SKIPPED;
    }

    //Called from CPU1 with renderMutex held (reads baseColour).
    void CommandProcessor::drawScanline(
            scanvideo_scanline_buffer_t *scanline_buffer,
            const std::vector<RleRun> &merged) {

        //We assume the `merged` vector is sorted and has no duplicates by
        //the time we get here.
        static const ScanlineTokens tokens = {
                .colorRun = COMPOSABLE_COLOR_RUN,
                .raw1p = COMPOSABLE_RAW_1P,
                .raw2p = COMPOSABLE_RAW_2P,
                .eolAlign = COMPOSABLE_EOL_ALIGN,
                .eolSkipAlign = COMPOSABLE_EOL_SKIP_ALIGN,
        };

        ScanlineEncodeResult result = encodeScanline(
                merged,
                (uint16_t *) scanline_buffer->data,
                (size_t) scanline_buffer->data_max * 2,
                tokens,
                maxDisplayWidth,
                (uint16_t) baseColour);

        if (!result.ok) {
            skipScanline(scanline_buffer);
            return;
        }
        if (result.truncated) {
            //Can't log here (core 1, IRQs off); inspect from a debugger or log elsewhere.
            truncatedScanlines = truncatedScanlines + 1;
        }

        scanline_buffer->data_used = result.wordsUsed;
        scanline_buffer->status = SCANLINE_OK;
    }

    void CommandProcessor::computeFrame() {
        mutex_enter_blocking(&pendingMutex);
        computeFrameLocked();
        mutex_exit(&pendingMutex);
    }

    void CommandProcessor::computeFrameLocked() {

        //Mark the frame as not computed while we are computing it (render skips lines meanwhile).
        mutex_enter_blocking(&renderMutex);
        isFrameComputed = false;
        mutex_exit(&renderMutex);

        for (const auto &g : rleRunsToProcess) {
            //Merge outside the render lock; it's the slow part.
            std::vector<RleRun> merged = mergeRuns(std::make_unique<std::vector<RleRun>>(g.second));

            mutex_enter_blocking(&renderMutex);
            auto existing = rleRunsForLine.find(g.first);
            if (existing != rleRunsForLine.end()) {
                existing->second.swap(merged); //merged now holds the old runs
            } else {
                rleRunsForLine.emplace(g.first, std::move(merged));
            }
            mutex_exit(&renderMutex);
            //Old runs (if any) freed here, outside renderMutex.
        }

        mutex_enter_blocking(&renderMutex);
        isFrameComputed = true;
        mutex_exit(&renderMutex);

        rleRunsToProcess.clear();
    }

    //Caller holds pendingMutex (uses lineBuffer).
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

    void CommandProcessor::setBaseColour(uint32_t color) {
        mutex_enter_blocking(&pendingMutex);
        mutex_enter_blocking(&renderMutex);
        this->baseColour = color;
        mutex_exit(&renderMutex);
        clearFrameLocked();
        mutex_exit(&pendingMutex);
    }

    void CommandProcessor::clearScanlines(uint16_t min, uint16_t max) {
        mutex_enter_blocking(&pendingMutex);
        for (int i = min; i <= max; i++) {
            std::vector<RleRun> old;
            mutex_enter_blocking(&renderMutex);
            auto line = rleRunsForLine.find(i);
            if (line != rleRunsForLine.end()) {
                line->second.swap(old);
            }
            mutex_exit(&renderMutex);
            //old freed here, outside renderMutex.
        }
        mutex_exit(&pendingMutex);
    }

    bool CommandProcessor::hasGraphicsToRender() {
        //TODO maybe check if each key is just an empty vector too?
        mutex_enter_blocking(&renderMutex);
        bool hasGraphics = !rleRunsForLine.empty();
        mutex_exit(&renderMutex);
        return hasGraphics;
    }

    bool CommandProcessor::getIsImmediateMode() {
        mutex_enter_blocking(&pendingMutex);
        bool immediate = isImmediateMode;
        mutex_exit(&pendingMutex);
        return immediate;
    }

    void CommandProcessor::setImmediateMode(bool immediateModeOn) {
        mutex_enter_blocking(&pendingMutex);
        this->isImmediateMode = immediateModeOn;
        mutex_exit(&pendingMutex);
    }

    void CommandProcessor::drawSolidColorScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour) {

        //Same shape as scanvideo.c's missing-scanline data: 6 halfwords, word-aligned.
        //COLOR_RUN + RAW_1P is 5 halfwords, so the end-of-line token sits on an odd
        //halfword and must be EOL_ALIGN (EOL_SKIP_ALIGN needs a word boundary).
        uint16_t *p = (uint16_t *) scanline_buffer->data;

        *p++ = COMPOSABLE_COLOR_RUN;
        *p++ = colour;
        *p++ = maxDisplayWidth - 3; //count-3 encoding: maxDisplayWidth colour pixels (as before)

        // black pixel to end line
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = 0;
        *p++ = COMPOSABLE_EOL_ALIGN;

        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        assert(scanline_buffer->data_used <= scanline_buffer->data_max);

        scanline_buffer->status = SCANLINE_OK;

    }


} // command
