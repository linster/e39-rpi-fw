//
// Created by stefan on 11/20/22.
//



#ifndef PICOTEMPLATE_BASESCANPROGRAM_H
#define PICOTEMPLATE_BASESCANPROGRAM_H


#include <cstdint>

namespace video {
    namespace scanProgram {
        namespace scanPrograms {

            class BaseScanProgram {

                //TODO some private class variables for mutexes and semaphors
            public:
                /** One-time setup code for the scan program, called
                 *  from main (via ScanProgrammanager)
                 */
                void init();

                //https://github.com/raspberrypi/pico-playground/blob/master/scanvideo/scanvideo_minimal/scanvideo_minimal.c
                //starts and stops the while loop on vga_main
                void startScanProgram();
                void stopScanProgram();
            private:
                void render_loop();
                //TODO a private method to get the vga timing.
            protected:

                /* The method subclasses use to draw scanlines, wrapped
                 * by the base class with all the necessary mutex setup
                 * and teardown code */
                virtual void render(uint32_t frame_num);


                uint16_t getDisplayHeightPx();
                uint16_t getDisplayWidthPx();
            };

        } // video
    } // scanProgram
} // scanPrograms

#endif //PICOTEMPLATE_BASESCANPROGRAM_H
