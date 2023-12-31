//
// Created by stefan on 9/1/23.
//

#include "NoopScanProgram.h"

namespace video::scanProgram::scanPrograms::noop {

    NoopScanProgram::NoopScanProgram(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
        init(logger);
    }

    std::string NoopScanProgram::getTag() {
        return "NoopScanProgram";
    }

    void NoopScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
        //scanvideo_timing_enable(false);
        //TODO we only ever scanvideo_timing_enable(true) from bootsplash and menu, so we need to test if it's actually
        //TODO possible to start -> clock ->  menu. TEST #1

        //TODO we may want to move the scan video timing out of the onStart()/onStop() and put it in the scanProgramManager
        //TOOD in onCpu1Setup().
    }

    void NoopScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void NoopScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP

        //TODO if this scan program runs after another scan program has started scanvideo_timing, we might
        //TODO be starving the DMA of something, causing it to lock up CPU1. I'm not sure why a lockup of CPU1 causes
        //TODO cpu0 to lock up too, but that could be related to a deadlock?

        //TODO what we may want to do here is to just always return a skipped scanline?
        if (scanline_buffer != nullptr) {
            skipScanline(scanline_buffer);
        }
    }

    void NoopScanProgram::skipScanline(scanvideo_scanline_buffer_t *scanline_buffer) {
        scanline_buffer->data_used = 0;
        scanline_buffer->status = SCANLINE_SKIPPED;
    }


} // noop