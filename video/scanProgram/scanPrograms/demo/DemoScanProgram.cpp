//
// Created by stefan on 9/1/23.
//

#include "DemoScanProgram.h"

namespace video::scanProgram::scanPrograms::demo {

    DemoScanProgram::DemoScanProgram(std::shared_ptr<pico::logger::BaseLogger> logger,
                                     std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib) {
        this->logger = logger;
        this->graphicsLib = graphicsLib;
        this->graphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );

        init(logger);
    }

    std::string DemoScanProgram::getTag() {
        return "DemoScanProgram";
    }

    void DemoScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");
        setupComputedFrame();
    }

    void DemoScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void DemoScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //render_flag_ua(scanline_buffer);
        render_text(scanline_buffer);

        render_computedFrame(scanline_buffer);
    }

    void DemoScanProgram::render_text(scanvideo_scanline_buffer_t *scanline_buffer) {


    }

    void DemoScanProgram::render_flag_ua(scanvideo_scanline_buffer_t *scanline_buffer) {
        //Let's make the Ukrainian flag here. Solid runs of scanline for blue, for half the height
        //Then, solid runs of yellow scanlines for the rest of the height.
        if (scanvideo_scanline_number(scanline_buffer->scanline_id) < getDisplayHeightPx() / 2) {
            graphicsLib->writeSolidColourScanline(scanline_buffer, graphicsLib->getPalette()[3]);
        } else {
            graphicsLib->writeSolidColourScanline(scanline_buffer, graphicsLib->getPalette()[14]);
        }
    }

    int32_t DemoScanProgram::single_color_scanline(uint32_t *buf, size_t buf_length, int width, uint32_t color16) {
        //https://github.com/raspberrypi/pico-playground/blob/master/scanvideo/scanvideo_minimal/scanvideo_minimal.c
        assert(buf_length >= 2);

        assert(width >= MIN_COLOR_RUN);
        // | jmp color_run | color | count-3 |  buf[0] =
        buf[0] = COMPOSABLE_COLOR_RUN | (color16 << 16);
        buf[1] = (width - MIN_COLOR_RUN) | (COMPOSABLE_RAW_1P << 16);
        // note we must end with a black pixel
        buf[2] = 0 | (COMPOSABLE_EOL_ALIGN << 16);

        return 3;
    }

    void DemoScanProgram::setupComputedFrame() {

        graphicsLib->drawEmptyRectangle(
                scanVideo::graphics::command::PxCoord(10,10),
                scanVideo::graphics::command::PxCoord(30,30),
                graphicsLib->getPalette()[14],
                1
        );


        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(50,5),
                scanVideo::graphics::command::PxCoord(120,70),
                graphicsLib->getPalette()[13]
        );

        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(50,50),
                scanVideo::graphics::command::PxCoord(80,80),
                graphicsLib->getPalette()[15]
        );

        graphicsLib->drawEmptyRectangle(
                scanVideo::graphics::command::PxCoord(60,60),
                scanVideo::graphics::command::PxCoord(90,90),
                graphicsLib->getPalette()[14],
                2
        );

        graphicsLib->drawEmptyRectangle(
                scanVideo::graphics::command::PxCoord(160,60),
                scanVideo::graphics::command::PxCoord(190,90),
                graphicsLib->getPalette()[14],
                2
        );

        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(60,95),
                scanVideo::graphics::command::PxCoord(190,95),
                graphicsLib->getPalette()[10],
                1
        );
        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(190,97),
                scanVideo::graphics::command::PxCoord(190,100),
                graphicsLib->getPalette()[10],
                1
        );
        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(190,97),
                scanVideo::graphics::command::PxCoord(191,97),
                graphicsLib->getPalette()[10],
                1
        );

        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(100,120),
                scanVideo::graphics::command::PxCoord(100,180),
                graphicsLib->getPalette()[6],
                1
        );

        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(120,120),
                scanVideo::graphics::command::PxCoord(120,180),
                graphicsLib->getPalette()[6],
                2
        );

        graphicsLib->drawText(
                "Hello, World!",
                scanVideo::graphics::command::PxCoord(181, 10),
                graphicsLib->getPalette()[15],
                1
                );
        graphicsLib->drawText(
                "AAA",
                scanVideo::graphics::command::PxCoord(181, 20),
                graphicsLib->getPalette()[15],
                2
                );
        graphicsLib->drawText(
                "BBBB",
                scanVideo::graphics::command::PxCoord(81, 80),
                graphicsLib->getPalette()[15],
                5
                );


        graphicsLib->drawLine(
                scanVideo::graphics::command::PxCoord(10,120),
                scanVideo::graphics::command::PxCoord(80,180),
                graphicsLib->getPalette()[6],
                1
        );

        graphicsLib->setUserFrameState(1);
    }

    void DemoScanProgram::render_computedFrame(scanvideo_scanline_buffer_t *scanline_buffer) {

        //We can check the frame number here, and optionally update state
        //to change the graphics on screen.

        graphicsLib->render_commandProcessed(scanline_buffer);
    }

} // demo