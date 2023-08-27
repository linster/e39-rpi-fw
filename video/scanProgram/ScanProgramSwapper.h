//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_SCANPROGRAMSWAPPER_H
#define PICOTEMPLATE_SCANPROGRAMSWAPPER_H

#include "ScanProgram.h"

namespace video::scanProgram {

        class ScanProgramSwapper {

        public:

            void swapTo(ScanProgram scanProgram);

            ScanProgram getCurrentScanProgram();


            //This is here as a way for the main core to queue up, via mutexes and shared state,
            //which scan program should be running on the secondary core.

            //This also needs a way for core-0 classes to know what the current scan program is
            //(so that knob turn knows to activate the pico menu only when it's displayed)
        };

    } // scanProgram

#endif //PICOTEMPLATE_SCANPROGRAMSWAPPER_H
