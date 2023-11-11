//
// Created by stefan on 11/20/22.
//

#include "ScanProgramSwapper.h"

namespace video::scanProgram {
        ScanProgramSwapper::ScanProgramSwapper(
                std::shared_ptr <pico::logger::BaseLogger> logger,
                std::shared_ptr <ScanProgramManager> scanProgramManager) {

            this->logger = logger;
            this->scanProgramManager = scanProgramManager;
        }

        void ScanProgramSwapper::swapTo(ScanProgram scanProgram) {

            std::string scanProgramName;
            switch (scanProgram) {
                case BOOT_SPLASH: scanProgramName = "BOOT_SPLASH"; break;
                case CLOCK: scanProgramName = "CLOCK"; break;
                case DEMO: scanProgramName = "DEMO"; break;
                case MENU: scanProgramName = "MENU"; break;
                case NOOP: scanProgramName = "NOOP"; break;
            }

            logger->d("ScanProgramSwapper", fmt::format("Swapping to scan program {}", scanProgramName));
            scanProgramManager->swapTo(scanProgram);
        }

        ScanProgram ScanProgramSwapper::getCurrentScanProgram() {
            return scanProgramManager->getCurrentScanProgram();
        }


    } // scanProgram