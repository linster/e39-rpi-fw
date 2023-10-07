//
// Created by stefan on 9/1/23.
//

#include "BootsplashScanProgram.h"

namespace video::scanProgram::scanPrograms::bootsplash {

    BootsplashScanProgram::BootsplashScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib) {
        this->logger = logger;
        this->graphicsLib = graphicsLib;
        this->graphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );

        init(logger);
    }

    std::string BootsplashScanProgram::getTag() {
        return "BootsplashScanProgram";
    }

    void BootsplashScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
    }

    void BootsplashScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void BootsplashScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP
    }
} // bootsplash