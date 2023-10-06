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
        //Let's make the Ukrainian flag here. Solid runs of scanline for blue, for half the height
        //Then, solid runs of yellow scalines for the rest of the height.


//        uint32_t color_blue = PICO_SCANVIDEO_PIXEL_FROM_RGB5(255, 255, 255);
//        uint32_t color_yellow = PICO_SCANVIDEO_PIXEL_FROM_RGB5(127, 127, 0);
//
//        uint16_t *p = (uint16_t *) scanline_buffer->data;
//
//        if (scanline_buffer->scanline_id > 525) {
//            //Somehow we are in the VBI
//            scanline_buffer->status = SCANLINE_SKIPPED;
//            return;
//        }
//
//        if (scanline_buffer->scanline_id == getDisplayHeightPx()) {
//            //We're on the last display line, and we need to change fields for interlacing.
//            scanline_buffer->status = SCANLINE_SKIPPED;
//            return;
//
//        }

//
//        if (scanline_buffer->scanline_id %2 != 0) {
//            scanline_buffer->status = SCANLINE_SKIPPED;
//            return;
//        }
//
//        *p++ = COMPOSABLE_COLOR_RUN;
//        if (scanline_buffer->scanline_id < 117) {
//            //Blue
//            *p++ = color_blue;
//        } else {
//            //Yellow
//            *p++ = color_yellow;
//        }
//
//        *p++ = 32;

//
//        *p++ = COMPOSABLE_COLOR_RUN;
//        *p++ = color_yellow;
//        *p++ = getDisplayWidthPx() - 3 - 1;// - 32;
//
//        // black pixel to end line
//        *p++ = COMPOSABLE_RAW_1P;
//        *p++ = PICO_SCANVIDEO_PIXEL_FROM_RGB5(0,0,0);
//
//
//        // end of line with alignment padding
//        *p++ = COMPOSABLE_EOL_SKIP_ALIGN;
//        *p++ = 0;
//
//        //Some pointer arithmetic... gross!
//        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
//        assert(scanline_buffer->data_used < scanline_buffer->data_max);

        uint32_t *buf = scanline_buffer->data;
        size_t buf_length = scanline_buffer->data_max;

        int l = scanvideo_scanline_number(scanline_buffer->scanline_id);
        uint16_t bgcolour = (uint16_t) l << 2;
        scanline_buffer->data_used = single_color_scanline(buf, buf_length, 32, bgcolour);
        scanline_buffer->status = SCANLINE_OK;
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