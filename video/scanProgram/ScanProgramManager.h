//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_SCANPROGRAMMANAGER_H
#define PICOTEMPLATE_SCANPROGRAMMANAGER_H

#include "pico/mutex.h"
#include "ScanProgram.h"

namespace video {
    namespace scanProgram {

        class ScanProgramManager {


            //This should get a reference to scan program swapper

        private:

            /** Guard the currently running scan program */
            mutex_t scanProgramStateMutex;




            mutex_t scanProgramMutex;


            void cpu0swapScanprogam(ScanProgram scanProgram);
            ScanProgram cpu0getCurrentScanprogram();

        public:


            void swapTo(ScanProgram scanProgram);

            ScanProgram getCurrentScanProgram();



        };



    } // video
} // scanProgram

#endif //PICOTEMPLATE_SCANPROGRAMMANAGER_H
