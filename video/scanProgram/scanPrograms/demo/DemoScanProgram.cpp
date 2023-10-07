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
    }

    void DemoScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void DemoScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {
        //render_flag_ua(scanline_buffer);
        render_text(scanline_buffer);
    }

    void DemoScanProgram::render_text(scanvideo_scanline_buffer_t *scanline_buffer) {

        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(50,50),
                scanVideo::graphics::command::PxCoord(80,80),
                graphicsLib->getPalette()[2]
                );
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

} // demo