//
// Created by stefan on 11/20/22.
//



#ifndef PICOTEMPLATE_BASESCANPROGRAM_H
#define PICOTEMPLATE_BASESCANPROGRAM_H

#include <memory>
#include <cstdint>
#include "pico/mutex.h"
#include "pico/scanvideo.h"
#include "../../pico_extras/src/common/pico_scanvideo/scanvideo.pio.h"
#include "../../../logging/BaseLogger.h"
#include "pico/scanvideo/composable_scanline.h"

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

                //todo, call the render method in here. without blocking
                //todo nope, we're doing rendering on cpu0. We have LIN interrupts on cpu1.
                void onCpu1Loop();


            private:

                std::shared_ptr<pico::logger::BaseLogger> logger;

                //Guard isScanProgramRunningMutex
                mutex_t isScanProgramRunningMutex;
                bool isScanProgramRunning;
                //End Guard: isScanProgramRunningMutex

                scanvideo_timing_t getScanProgramTiming();
                scanvideo_mode_t getScanVideoMode();

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
                constexpr static const scanvideo_timing_t vga_timing_bmbt = {
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

                constexpr static const scanvideo_mode_t mode_bmbt = {
                        .default_timing = &vga_timing_bmbt,
                        .pio_program = &video_24mhz_composable,
                        .width = 400,
                        .height = 234,
                        .xscale = 1,
                        .yscale = 1,
                        .yscale_denominator = 1
                };

            protected:


                bool shouldKeepRunning();

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
