//
// Created by stefan on 9/1/23.
//

#include "MenuScanProgram.h"

namespace video::scanProgram::scanPrograms::menu {


    MenuScanProgram::MenuScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib) {
        this->logger = logger;
        this->graphicsLib = graphicsLib;
        this->graphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );
        init(logger);
    }

    std::string MenuScanProgram::getTag() {
        return "MenuScanProgram";
    }

    void MenuScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
    }

    void MenuScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void MenuScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //NOOP
    }

} // menu