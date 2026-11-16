//
// Writing a solid-colour scanline, and why scan programs must never return an empty one.
//

#ifndef PICOTEMPLATE_SOLIDSCANLINE_H
#define PICOTEMPLATE_SOLIDSCANLINE_H

#include <cstdint>
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"

namespace video::scanProgram {

    /**
     * Fill a scanline buffer with one full-width run of `colour` plus the usual black end pixel.
     * Same shape as scanvideo.c's own missing-scanline data: COLOR_RUN, colour, width-3, RAW_1P, 0,
     * EOL_ALIGN (6 halfwords; EOL_ALIGN because it starts on an odd halfword).
     *
     * Use this (with colour 0 for black) instead of returning a "skipped" scanline. scanvideo's DPI
     * driver never looks at scanline_buffer->status, so SCANLINE_SKIPPED with data_used = 0 is still
     * DMA'd as an empty line: no pixels and no end-of-line token, so the scanline state machine stalls
     * mid-line waiting for data. At the next line start scanvideo finds it away from its `wait 1 irq 4`
     * and "recovers" it by forcing that instruction in with pio_sm_exec, which can leave it wedged
     * (see ScanProgramManager::recoverWedgedScanlineStateMachine). Measured on the bench: the clock,
     * which skipped its whole top region, triggered that recovery on ~12,000 lines a second; with
     * solid black lines, none.
     */
    inline void writeSolidScanline(scanvideo_scanline_buffer_t *scanline_buffer, uint16_t width, uint16_t colour) {
        uint16_t *p = (uint16_t *) scanline_buffer->data;
        *p++ = COMPOSABLE_COLOR_RUN;
        *p++ = colour;
        *p++ = width - 3;
        *p++ = COMPOSABLE_RAW_1P;
        *p++ = 0;
        *p++ = COMPOSABLE_EOL_ALIGN;
        scanline_buffer->data_used = ((uint32_t *) p) - scanline_buffer->data;
        scanline_buffer->status = SCANLINE_OK;
    }

} // video::scanProgram

#endif //PICOTEMPLATE_SOLIDSCANLINE_H
