//
// Created by stefan on 9/1/23.
//

#include "ClockScanProgram.h"

namespace video::scanProgram::scanPrograms::clock {

    ClockScanProgram::ClockScanProgram(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
        init(logger);
    }

    std::string ClockScanProgram::getTag() {
        return "ClockScanProgram";
    }

    void ClockScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
    }

    void ClockScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void ClockScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP
    }

} // clock