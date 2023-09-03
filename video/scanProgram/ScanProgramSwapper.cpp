//
// Created by stefan on 11/20/22.
//

#include "ScanProgramSwapper.h"

namespace video {
    namespace scanProgram {
        ScanProgramSwapper::ScanProgramSwapper(
                std::shared_ptr <pico::logger::BaseLogger> logger,
                std::shared_ptr <ScanProgramManager> scanProgramManager) {

            this->logger = logger;
            this->scanProgramManager = scanProgramManager;
        }

        void ScanProgramSwapper::swapTo(ScanProgram scanProgram) {
            logger->d("ScanProgramSwapper", "Swapping to scan program");
            scanProgramManager->swapTo(scanProgram);
        }

        ScanProgram ScanProgramSwapper::getCurrentScanProgram() {
            return scanProgramManager->getCurrentScanProgram();
        }


    } // video
} // scanProgram