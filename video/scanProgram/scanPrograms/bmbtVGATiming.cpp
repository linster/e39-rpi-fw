//
// Created by stefan on 11/20/22.
//

#include "pico/scanvideo.h"

#include "BaseScanProgram.h"

namespace video::scanProgram::scanPrograms {

    scanvideo_timing_t BaseScanProgram::getScanProgramTiming() {

        //https://github.com/linster/e39-rpi/blob/master/systemImage/builder/config_txt/config.400x234.menu.ntsc.txt
        //hdmi_timings=400 0 20 29 59 234 0 7 3 19 0 0 0 50 1 7867500 3
        //hdmi_timings=
        // h_active_pixels = 400
        // h_sync_polarity = 0
        // h_front_porch = 20
        // h_sync_pulse = 29
        // h_back_porch = 59
        // v_active_lines = 234
        // v_sync_polarity = 0
        // v_front_porch = 7
        // v_sync_pulse = 3
        // vback_porch = 19
        // v_sync_offset_a = 0
        // v_sync_offset_b = 0
        // pixel_rep = 0
        // frame_rate (Hz) = 50
        // interlaced = 1
        // pixel_freq = 7867500
        // aspect_ratio (16:9) = 3

        const scanvideo_timing_t vga_timing_bmbt = {
                .clock_freq = 7867500,

                .h_active = 400, /* OK */
                .v_active = 234, /* OK */

                .h_front_porch = 20, /* OK */
                .h_pulse = 29,
                .h_total = 508, /* Active + front + back + sync = 400 + 20 + 59 + 29 = 508 */
                .h_sync_polarity = 1,

                .v_front_porch = 7, /* OK */
                .v_pulse = 3, /* OK */
                .v_total = 263, /* Active + front + back + sync = 234 + 7 + 19 + 3 = 263 */
                .v_sync_polarity = 0,

                .enable_clock = 0,
                .clock_polarity = 0,

                .enable_den = 0
        };
        return vga_timing_bmbt;
    }
}

