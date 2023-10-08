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
        switch (bootSplashType) {
            case LINSTER_OS:
                onScanProgramStart_drawLinsterOS();
                break;
            case BMW:
                onScanProgramStart_BMW();
                break;
            case GOOSE:
                onScanProgramStart_Goose();
                break;
        }
    }

    void BootsplashScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
        graphicsLib->clearFrame();
    }

    void BootsplashScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        switch (bootSplashType) {
            case LINSTER_OS:
                render_LinsterOS(scanline_buffer);
                break;
            case BMW:
                render_BMW(scanline_buffer);
                break;
            case GOOSE:
                render_Goose(scanline_buffer);
                break;
        }
    }

    void BootsplashScanProgram::render_LinsterOS(scanvideo_scanline_buffer_t *scanline_buffer) {
        graphicsLib->render_commandProcessed(scanline_buffer);
    }

    void BootsplashScanProgram::render_BMW(scanvideo_scanline_buffer_t *scanline_buffer) {
        graphicsLib->render_commandProcessed(scanline_buffer);
    }

    void BootsplashScanProgram::render_Goose(scanvideo_scanline_buffer_t *scanline_buffer) {
        graphicsLib->render_commandProcessed(scanline_buffer);
    }

    void BootsplashScanProgram::onScanProgramStart_drawLinsterOS() {

        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(1,1),
                scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 2,getDisplayHeightPx() - 40),
                graphicsLib->getPalette()[15]
        );

    }

    void BootsplashScanProgram::onScanProgramStart_BMW() {

    }

    void BootsplashScanProgram::onScanProgramStart_Goose() {

    }
} // bootsplash