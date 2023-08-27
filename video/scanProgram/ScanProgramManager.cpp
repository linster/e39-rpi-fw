//
// Created by stefan on 11/20/22.
//

#include "ScanProgramManager.h"

namespace video {
    namespace scanProgram {

        ScanProgram ScanProgramManager::getCurrentScanProgram() {
            return cpu0getCurrentScanprogram();
        }

        void ScanProgramManager::cpu0swapScanprogam(ScanProgram scanProgram) {
            cpu0swapScanprogam(scanProgram);
        }



    } // video
} // scanProgram