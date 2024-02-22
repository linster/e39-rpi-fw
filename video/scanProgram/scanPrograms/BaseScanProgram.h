//
// Created by stefan on 11/20/22.
//



#ifndef PICOTEMPLATE_BASESCANPROGRAM_H
#define PICOTEMPLATE_BASESCANPROGRAM_H

#include <memory>
#include <cstdint>
#include "pico/mutex.h"
#include "pico/scanvideo.h"
//#include "../../pico_extras/src/common/pico_scanvideo/scanvideo.pio.h"
#include <logging/BaseLogger.h>
#include "pico/scanvideo/composable_scanline.h"


#include "pico.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"

#include "fmt/format.h"
#include "pico/sync.h"

//Un-comment this if you want to test on a VGA monitor
//and not on the real BMBT screen. Remove the VGA-RGsB board
//from the video path when using this.
//#define TEST_VGA_MONITOR

#ifdef TEST_VGA_MONITOR
#include "pico/scanvideo/scanvideo_base.h"
#endif


namespace video::scanProgram::scanPrograms {

            class BaseScanProgram {

            public:
                /** One-time setup code for the scan program, called
                 *  from main (via ScanProgrammanager)
                 */
                void init(
                        std::shared_ptr<pico::logger::BaseLogger> logger
                        );

                //https://github.com/raspberrypi/pico-playground/blob/master/scanvideo/scanvideo_minimal/scanvideo_minimal.c
                //starts and stops the while loop on vga_main
                //Called from within a mutex in ScanProgramManager, so be quick!
                void startScanProgram();
                void stopScanProgram();

                //Called only by ApplicationContainer, via ScanProgramManager.
                void cpu0setup();
                void onCpu0Loop();
                void cpu1Setup();
                void onCpu1Loop();

                void callRender();

                #ifdef TEST_VGA_MONITOR
                    //This is a copy of vga_timing_640x480_60_default but as constexpr.
                    constexpr static scanvideo_timing_t vga_timing_bmbt =
                            {

                                    .clock_freq = 25000000,

                                    .h_active = 640,
                                    .v_active = 480,

                                    .h_front_porch = 16,
                                    .h_pulse = 64,
                                    .h_total = 800,
                                    .h_sync_polarity = 1,

                                    .v_front_porch = 1,
                                    .v_pulse = 2,
                                    .v_total = 523,
                                    .v_sync_polarity = 1,

                                    .enable_clock = 0,
                                    .clock_polarity = 0,

                                    .enable_den = 0
//                                    .clock_freq = 24000000,
//
//                                    .h_active = 640,
//                                    .v_active = 480,
//
//                                    .h_front_porch = 72,
//                                    .h_pulse = 96,
//                                    .h_total = 928,
//                                    .h_sync_polarity = 1,
//
//                                    .v_front_porch = 8,
//                                    .v_pulse = 2,
//                                    .v_total = 518,
//                                    .v_sync_polarity = 1,
//
//                                    .enable_clock = 0,
//                                    .clock_polarity = 0,
//
//                                    .enable_den = 0
                            };
                    //This is a copy of and vga_mode_320x240_60 but as constexpr.
                    constexpr static scanvideo_mode_t mode_bmbt = {
                            .default_timing = &vga_timing_bmbt,
                            .pio_program = &video_24mhz_composable,
                            .width = 320,
                            .height = 240,
                            .xscale = 2,
                            .yscale = 2,
                    };
                #else
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


                    //Let's try this line for DE PAL
                    //hdmi_timings=
                    // h_active = 800
                    // h_sync_polarity = 0
                    // h_front_porch = 31
                    // h_sync_pulse = 56
                    // h_back_porch = 129
                    // v_active = 288
                    // v_sync_polarity = 0
                    // v_front_porch = 1
                    // v_sync_pulse = 3
                    // v_back_porch = 20
                    // 0 0 0 50 1 15625000 3

        //hdmi_timings=400 0 20 29 59 234 0 7 3 19 0 0 0 50 1 7867500 3
                    constexpr static const uint16_t h_active = 400;
                    constexpr static const uint16_t h_front_porch = 20;
                    constexpr static const uint16_t h_pulse = 29;
                    constexpr static const uint16_t h_back = 59;

                    constexpr static const uint16_t v_active = 234;
                    constexpr static const uint16_t v_front = 7;
                    constexpr static const uint16_t v_sync = 3;
                    constexpr static const uint16_t v_back = 19;


            constexpr static const scanvideo_timing_t vga_timing_bmbt = {
                            .clock_freq = 7867500, //Original.
//                            .clock_freq = 15625000, //CPU needs to go up to 126MHz.

                            //TODO Almost works. We can make the same active width as
                            //TODO before by compensating on h_active. Need to make sure
                            //TODO the h porches and pulses are the same width as before.
                            //TODO then take up the slack with h_active.
                            .h_active = h_active, /* OK */
                            .v_active = v_active, /* OK */

                            .h_front_porch = h_front_porch, /* OK */
                            .h_pulse = h_pulse,
                            //.h_total = 508, /* Active + front + back + sync = 400 + 20 + 59 + 29 = 508 */
                            .h_total = h_active + h_front_porch + h_back + h_pulse, /* Active + front + back + sync = 400 + 20 + 59 + 29 = 508 */
                            .h_sync_polarity = 1,

                            //TODO the porches and pulse width might also have to be scaled.
                            //TODO I know we want 234 active lines to avoid rolling.
                            .v_front_porch = v_front, /* OK */
                            .v_pulse = v_sync, /* OK */
                            .v_total = v_active + v_front + v_back + v_sync, /* Active + front + back + sync = 234 + 7 + 19 + 3 = 263 */
                            .v_sync_polarity = 1,

                            .enable_clock = 0,
                            .clock_polarity = 0,

                            .enable_den = 0
                    };

                    constexpr static const scanvideo_mode_t mode_bmbt = {
                            .default_timing = &vga_timing_bmbt,
                            .pio_program = &video_24mhz_composable,
                            .width = h_active,
                            .height = v_active,
                            .xscale = 1,
                            .yscale = 1,
                            .yscale_denominator = 1
                    };

                #endif

            private:

                critical_section_t renderCs;
                std::shared_ptr<pico::logger::BaseLogger> logger;

                scanvideo_timing_t getScanProgramTiming();
                scanvideo_mode_t getScanVideoMode();

            protected:

                /* The method subclasses use to draw scanlines, wrapped
                 * by the base class with all the necessary mutex setup
                 * and teardown code */
                virtual void render(scanvideo_scanline_buffer_t *scanline_buffer) = 0;

                /** Called when the ScanProgramSwapper stops and starts our scan programs */
                virtual void onScanProgramStart() = 0;
                virtual void onScanProgramStop() = 0;

                uint16_t getDisplayHeightPx();
                uint16_t getDisplayWidthPx();

                virtual std::string getTag() = 0;

            };

        } // scanPrograms

#endif //PICOTEMPLATE_BASESCANPROGRAM_H
