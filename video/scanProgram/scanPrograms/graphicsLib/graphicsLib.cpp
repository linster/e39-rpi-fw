//
// Created by stefan on 10/7/23.
//

#include "graphicsLib.h"

namespace video::scanProgram {
    uint32_t *graphicsLib::getPalette() {
        return (uint32_t *) &pallet;
    }


    void graphicsLib::setDisplayMetrics(DisplayMetrics displayMetrics) {
        this->displayMetrics = displayMetrics;
        this->haveDisplayMetrics = true;
    }

    void graphicsLib::requireDisplayMetrics() {
        if (!haveDisplayMetrics) {
            panic("No display metrics");
        }
    }

    void graphicsLib::writeSolidColourScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint32_t colour) {
        requireDisplayMetrics();

        uint16_t *p = (uint16_t *) scanline_buffer->data;

        *p++ = COMPOSABLE_COLOR_RUN;
        uint32_t color = colour;
        *p++ = color;
        *p++ = displayMetrics.getDisplayWidthPx() - 3;

        // 32 * 3, so we should be word aligned
//        assert(!(3u & (uintptr_t) p));

        // black pixel to end line
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = 0;
        // end of line with alignment padding
        *p++ = COMPOSABLE_EOL_SKIP_ALIGN;
        *p++ = 0;

        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        assert(scanline_buffer->data_used < scanline_buffer->data_max);

        scanline_buffer->status = SCANLINE_OK;

    }
} // scanProgram