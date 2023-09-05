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


        uint32_t color_blue = PICO_SCANVIDEO_PIXEL_FROM_RGB5(0, 0, 255);
        uint32_t color_yellow = PICO_SCANVIDEO_PIXEL_FROM_RGB5(255, 255, 0);

        uint16_t *p = (uint16_t *) scanline_buffer->data;

        *p++ = COMPOSABLE_COLOR_RUN;
//        if (scanline_buffer->scanline_id < 117) {
            //Blue
            *p++ = color_blue;
//        } else {
            //Yellow
//            *p++ = color_yellow;
//        }
        // black pixel to end line
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = PICO_SCANVIDEO_PIXEL_FROM_RGB5(0,0,0);

        // end of line with alignment padding
        *p++ = COMPOSABLE_EOL_SKIP_ALIGN;
        *p++ = 0;

        //Some pointer arithmetic... gross!
        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);

        scanline_buffer->status = SCANLINE_OK;
    }

} // demo