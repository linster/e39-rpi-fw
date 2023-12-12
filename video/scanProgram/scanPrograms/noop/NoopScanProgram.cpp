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
    }

    void NoopScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void NoopScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP
    }


} // noop