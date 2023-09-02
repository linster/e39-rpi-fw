//
// Created by stefan on 9/1/23.
//

#include "DemoScanProgram.h"

namespace video::scanProgram::scanPrograms::demo {

    DemoScanProgram::DemoScanProgram(std::shared_ptr<pico::logger::BaseLogger> logger) {
        this->logger = logger;
        init(logger);
    }

    std::string DemoScanProgram::getTag() {
        return "DemoScanProgram";
    }

    void DemoScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
    }

    void DemoScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void DemoScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP

        //Let's make Ukrainian flag here. Solid runs of scanline for blue, for half the height
        //Then, solid runs of yellow scalines for the rest of the height.

        //TODO
        //TODO but before we do that, let's setup the factory for all this stuff.
    }

} // demo