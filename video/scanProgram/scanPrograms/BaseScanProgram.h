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
                void startScanProgram();
                void stopScanProgram();

                //Called only by ApplicationContainer, via ScanProgramManager.
                void cpu0setup();
                void onCpu0Loop();
                void cpu1Setup();

                //todo, call the render method in here. without blocking
                void onCpu1Loop();


                /** Called when the ScanProgramSwapper stops and starts our scan programs */
                void onScanProgramStart();
                void onScanProgramStop();
            private:

                std::shared_ptr<pico::logger::BaseLogger> logger;

                //TODO a semaphore around this isRunning boolean
                mutex_t isScanProgramRunningMutex;
                bool isScanProgramRunning;

                void render_loop();
                //TODO a private method to get the vga timing.

                scanvideo_timing_t getScanProgramTiming();

                /** Returns true if the scan program should keep running, false if it should end it's render loop */
                bool shouldKeepRunning();

            protected:

                //TODO We want two video planes.


                /* The method subclasses use to draw scanlines, wrapped
                 * by the base class with all the necessary mutex setup
                 * and teardown code */
                virtual void render(uint32_t frame_num);



                uint16_t getDisplayHeightPx();
                uint16_t getDisplayWidthPx();

            };

        } // scanPrograms

#endif //PICOTEMPLATE_BASESCANPROGRAM_H
