//
// Created by stefan on 11/20/22.
//

#include "ScanProgramManager.h"

namespace video {
    namespace scanProgram {

        void ScanProgramManager::swapTo(ScanProgram scanProgram) {
            //TODO Can be called from either CPU, so mutex it
        }

        ScanProgram ScanProgramManager::getCurrentScanProgram() {
            return cpu0getCurrentScanprogram();
        }

        void ScanProgramManager::cpu0swapScanprogam(ScanProgram scanProgram) {

        }


        void ScanProgramManager::cpu0setup() {
            if (classIsNoOp) { return; }

        }

        void ScanProgramManager::cpu1setup() {
            if (classIsNoOp) { return; }


        }

        void ScanProgramManager::onCpu0Loop() {
            if (classIsNoOp) { return; }

            //TODO delegate to the running scan program.
            //TODO if the running scan progra mhas changed, stop the old one, and start the new one.

        }

        void ScanProgramManager::onCpu1Loop() {
            if (classIsNoOp) { return; }


        }

        ScanProgramManager::ScanProgramManager(
                std::shared_ptr <pico::logger::BaseLogger> logger,
                std::shared_ptr <video::scanProgram::scanPrograms::noop::NoopScanProgram> noopScanProgram,
                std::shared_ptr <video::scanProgram::scanPrograms::menu::MenuScanProgram> menuScanProgram,
                std::shared_ptr <video::scanProgram::scanPrograms::demo::DemoScanProgram> demoScanProgram,
                std::shared_ptr <video::scanProgram::scanPrograms::clock::ClockScanProgram> clockScanProgram,
                std::shared_ptr <video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram> bootsplashScanProgram) {

            this->logger = logger;
            this->noopScanProgram = noopScanProgram;
            this->menuScanProgram = menuScanProgram;
            this->demoScanProgram = demoScanProgram;
            this->clockScanProgram = clockScanProgram;
        }
    } // video
} // scanProgram