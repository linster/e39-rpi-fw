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

        uint16_t logoSize = 160;

        uint16_t topScanline = 32;
        uint16_t startx = 200 - (logoSize / 2);
        uint16_t logoCenterX = 200;

        PxCoord topLeft = PxCoord(startx, topScanline);
        PxCoord bottomRight = PxCoord(topLeft.getX() + logoSize, topLeft.getY() + logoSize);


        uint32_t color_outlineTriangle = PICO_COLOR_FROM_RGB2(1, 1, 1);
        uint32_t color_water = PICO_COLOR_FROM_RGB2(0, 0, 3);
        uint32_t color_letters = PICO_COLOR_FROM_RGB2(1, 1, 1);

//        uint32_t color_mountainRocks = PICO_COLOR_FROM_RGB2(3, 3, 3);
        uint32_t color_mountainRocks = 0x2A;// 10_10_10;
        uint32_t color_mountainSnow = PICO_COLOR_FROM_RGB2(3, 3, 3);

//        graphicsLib->setImmediateMode(false);
//        graphicsLib->contributeRleRuns(runs);
        graphicsLib->drawFilledRectangle(
                topLeft,
                bottomRight,
                0//PICO_COLOR_FROM_RGB2(2, 2, 2)
                );

//        graphicsLib->drawLine(
//                PxCoord(logoCenterX, topScanline),
//                PxCoord(logoCenterX, bottomRight.getY()),
//                PICO_COLOR_FROM_RGB2(3, 3, 3),
//                1
//                );

        uint8_t outerTriangleThickness = 5;
        PxCoord outerTriangle_topLeft = PxCoord(topLeft.getX() + (logoSize / 8), topLeft.getY());
        PxCoord outerTriangle_topRight = PxCoord(bottomRight.getX() - (logoSize / 8), topLeft.getY());



        //draw the water
        uint16_t waterNablaTopY = bottomRight.getY() - ((bottomRight.getY() - topLeft.getY()) * 0.25);
        uint8_t waterNablaWidth = 13;
        graphicsLib->drawNabla(
                bottomRight.getY() - outerTriangleThickness,
                PxCoord(logoCenterX - waterNablaWidth, waterNablaTopY),
                PxCoord(logoCenterX + waterNablaWidth, waterNablaTopY),
                true,
                1,
                color_water
        );
        // end draw the water

        //mountain trapezoid
            uint16_t mountainTrapezoidTopY = waterNablaTopY - 36;
            uint16_t mountainTrapezoidBottomY = waterNablaTopY - 1;
            //draw the left half of the mountain trapezoid
            graphicsLib->drawLine(
                    PxCoord(logoCenterX - (2 * waterNablaWidth), mountainTrapezoidTopY),
                    PxCoord(logoCenterX - waterNablaWidth, mountainTrapezoidBottomY),
                    color_mountainRocks,
                    26
                    );
            //end draw the left half of the mountain trapezoid

            //draw the right half of the mountain trapezoid
            graphicsLib->drawNabla(
                    mountainTrapezoidBottomY,
                    PxCoord(logoCenterX, mountainTrapezoidTopY),
                    PxCoord(logoCenterX + (2* waterNablaWidth), mountainTrapezoidTopY),
                    true,
                    1,
                    color_mountainRocks
                    );
            //end draw the right half of the mountain trapezoid
        //end mountain trapezoid

        //draw mountain top rocks
        uint8_t mountainTopY = topScanline + outerTriangleThickness + (
                0.3 * (mountainTrapezoidTopY - (topScanline + outerTriangleThickness))
                );
        graphicsLib->drawDelta(
                mountainTopY,
                PxCoord(logoCenterX - (2 * waterNablaWidth), mountainTrapezoidTopY),
                PxCoord(logoCenterX + (2 * waterNablaWidth), mountainTrapezoidTopY),
                true,
                1,
                color_mountainRocks
                );
        //end draw mountain top rocks

        //draw the mountain top snow
        uint8_t mountainSnowBottomWidth = 10;
        uint8_t mountainSnowBottomY = topScanline + outerTriangleThickness + (
                0.55 * (mountainTrapezoidTopY - (topScanline + outerTriangleThickness))
        );
        graphicsLib->drawDelta(
                mountainTopY,
                PxCoord(logoCenterX - (mountainSnowBottomWidth), mountainSnowBottomY),
                PxCoord(logoCenterX + (mountainSnowBottomWidth), mountainSnowBottomY),
                true,
                1,
                color_mountainSnow
                );
        //end draw the mountain top snow


        //left rock peak
        uint8_t rockPeakTop = mountainSnowBottomY - 3;
        graphicsLib->drawDelta(
                rockPeakTop,
                PxCoord(logoCenterX - (mountainSnowBottomWidth), mountainSnowBottomY),
                PxCoord(logoCenterX, mountainSnowBottomY),
                true,
                1,
                color_mountainRocks
                );
        //end left rock peak

        //right rock peak
        graphicsLib->drawDelta(
                rockPeakTop,
                PxCoord(logoCenterX, mountainSnowBottomY),
                PxCoord(logoCenterX + (mountainSnowBottomWidth), mountainSnowBottomY),
                true,
                1,
                color_mountainRocks
        );
        //end right rock peak

        //draw 3

            //horizontal line top
            graphicsLib->drawLine(
                    PxCoord(outerTriangle_topLeft.getX() + (2* outerTriangleThickness), topScanline + ( 2 * outerTriangleThickness)),
                    PxCoord(logoCenterX - ( outerTriangleThickness), topScanline + ( 2 * outerTriangleThickness)),
                    color_letters,
                    outerTriangleThickness / 2
                    );
            //end horizontal line top

            //diagonal line top
            graphicsLib->drawLine(
                    PxCoord(logoCenterX - ( outerTriangleThickness), topScanline + ( 2 * outerTriangleThickness)),
                    PxCoord(outerTriangle_topLeft.getX() + (4* outerTriangleThickness), mountainTopY),
                    color_letters,
                    3
            );
            //end diagonal line top

            //diagonal line bottom
            graphicsLib->drawLine(
                    PxCoord(logoCenterX - ( outerTriangleThickness) - 5, mountainTopY),
                    PxCoord(logoCenterX - 28, mountainTrapezoidTopY - 16),
                    color_letters,
                    3
            );
            //end diagonal line bottom

            //horizontal line bottom
            graphicsLib->drawLine(
                    PxCoord(outerTriangle_topLeft.getX() + (4* outerTriangleThickness), mountainTopY),
                    PxCoord(logoCenterX - ( outerTriangleThickness) - 5, mountainTopY),
                    color_letters,
                    outerTriangleThickness / 2
            );
            //end horizontal line bottom

        //end draw 3

        //draw H
            //draw horizontal line
            graphicsLib->drawLine(
                    PxCoord(logoCenterX + ( outerTriangleThickness)+ 3, topScanline + ( 2 * outerTriangleThickness)),
                    PxCoord(outerTriangle_topRight.getX() - (2* outerTriangleThickness), topScanline + ( 2 * outerTriangleThickness)),
                    color_letters,
                    outerTriangleThickness / 2
            );
            //end draw horizontal line

            //draw vertical line
            graphicsLib->drawLine(
                    PxCoord(logoCenterX + ( outerTriangleThickness) + 3, topScanline + ( 2 * outerTriangleThickness)),
                    PxCoord(logoCenterX + ( outerTriangleThickness) + 3, mountainTopY),
                    color_letters,
                    3
                    );
            //end draw vertical line

            //draw diagonal line
            graphicsLib->drawLine(
                    PxCoord(logoCenterX + ( outerTriangleThickness) + 3, mountainTopY),
                    PxCoord(logoCenterX + 28 - 2, mountainTrapezoidTopY - 16),
                    color_letters,
                    3
                    );
            //end draw diagonal line
        //end draw H

        graphicsLib->drawLine(
                outerTriangle_topLeft,
                outerTriangle_topRight,
                color_outlineTriangle,
                outerTriangleThickness
                );

        PxCoord outerTriangleLeftSideTopLeft = PxCoord(
                outerTriangle_topLeft.getX() - outerTriangleThickness,
                outerTriangle_topLeft.getY()
                );
        for (int i = 0; i < outerTriangleThickness; i++) {
            graphicsLib->drawLine(
                    outerTriangleLeftSideTopLeft,
                    PxCoord(logoCenterX, bottomRight.getY() - i),
                    color_outlineTriangle,
                    1
                    );
            outerTriangleLeftSideTopLeft = PxCoord(
                    outerTriangle_topLeft.getX() - outerTriangleThickness + i,
                    outerTriangle_topLeft.getY()
                    );
        }

        PxCoord outerTriangleRightSideTopRight = PxCoord(
                outerTriangle_topRight.getX() + outerTriangleThickness,
                outerTriangle_topRight.getY()
                );
        for (int i = outerTriangleThickness; i >0 ; i--) {
            graphicsLib->drawLine(
                    outerTriangleRightSideTopRight,
                    PxCoord(logoCenterX, bottomRight.getY() - i),
                    color_outlineTriangle,
                    1
                    );
            outerTriangleRightSideTopRight = PxCoord(
                    outerTriangle_topRight.getX() + outerTriangleThickness - i,
                    outerTriangle_topRight.getY()
            );
        }



//        graphicsLib->drawText(
//                "WAT",
//                topLeft,
//                PICO_COLOR_FROM_RGB2(3, 3, 0),
//                2
//                );

//        graphicsLib->setImmediateMode(true);
//        graphicsLib->computeFrame();

    }
} // bootsplash