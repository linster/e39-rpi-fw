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
        scanvideo_timing_enable(true);
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
        switch (bootSplashType) {
            case LINSTER_OS:
                //Don't clear the scan program because menu draws on top of it.
                break;
            case BMW:
            case GOOSE:
                graphicsLib->clearFrame();
                break;
        }
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
        drawLinsterOs(
                graphicsLib,
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
                );
    }


    void BootsplashScanProgram::drawLinsterOs(
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib,
            DisplayMetrics displayMetrics
            ) {
        uint8_t topColourHeight = 40;
        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(1,1),
                scanVideo::graphics::command::PxCoord(displayMetrics.getDisplayWidthPx() - 2, displayMetrics.getDisplayHeightPx() - topColourHeight),
                graphicsLib::LINOS_BACKGROUND
        );

        uint8_t linOs_x = 5;
        uint8_t linOs_y = 20;
        uint8_t linOs_height = 2;
        std::string linOs_text = std::string("Linster OS");
        graphicsLib->drawText(
                linOs_text,
                scanVideo::graphics::command::PxCoord(linOs_x + 3, linOs_y + 2),
                graphicsLib->getPalette()[0],
                linOs_height
        );
        graphicsLib->drawText(
                linOs_text,
                scanVideo::graphics::command::PxCoord(linOs_x, linOs_y),
                graphicsLib->getPalette()[15],
                linOs_height
        );

        std::string automotive_text = std::string("Automotive");
        uint8_t automotive_x = linOs_x + (8 * linOs_height);
        uint8_t automotive_y = linOs_y + (8 * linOs_height) + 8;
        uint8_t automotive_height = 1;
        graphicsLib->drawText(
                "Automotive",
                scanVideo::graphics::command::PxCoord(automotive_x, automotive_y),
                graphicsLib->getPalette()[15],
                automotive_height
        );

        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(1,displayMetrics.getDisplayHeightPx() - topColourHeight),
                scanVideo::graphics::command::PxCoord(displayMetrics.getDisplayWidthPx() - 2,displayMetrics.getDisplayHeightPx() - 2),
                graphicsLib->getPalette()[14]
        );

        uint32_t copyrightColour = graphicsLib::LINOS_BACKGROUND;
        graphicsLib->drawText(
                "e39-Rpi",
                scanVideo::graphics::command::PxCoord(10, displayMetrics.getDisplayHeightPx() - topColourHeight + 8),
                copyrightColour,
                1
        );

        graphicsLib->drawText(
                "  Stefan Martynkiw 2019-2024",
                scanVideo::graphics::command::PxCoord(10, displayMetrics.getDisplayHeightPx() - topColourHeight + 10 + 10),
                copyrightColour,
                1
        );

        graphicsLib->drawTextSpecialCharacter(
                graphicsLib::SPECIAL_CHARACTER_COPYRIGHT,
                scanVideo::graphics::command::PxCoord(20, displayMetrics.getDisplayHeightPx() - topColourHeight + 10 + 10),
                copyrightColour,
                1
        );
    }
    void BootsplashScanProgram::onScanProgramStart_BMW() {

    }

    void BootsplashScanProgram::onScanProgramStart_Goose() {

//        std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>> runs = std::map<uint16_t, std::vector<scanVideo::graphics::command::RleRun>>();
        uint16_t topScanline = 58;
        uint16_t bottomScanline = topScanline + 128;

        uint32_t color_outlineTriange = PICO_COLOR_FROM_RGB2(1, 1, 1);
        uint32_t color_water = PICO_COLOR_FROM_RGB2(0, 0, 3);
        uint32_t color_letters = PICO_COLOR_FROM_RGB2(0, 3, 0);


//        graphicsLib->setImmediateMode(false);
//        graphicsLib->contributeRleRuns(runs);
        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(100, topScanline),
                scanVideo::graphics::command::PxCoord(200, 90),
                PICO_COLOR_FROM_RGB2(2, 2, 2)
                );
//        graphicsLib->setImmediateMode(true);
//        graphicsLib->computeFrame();

    }
} // bootsplash