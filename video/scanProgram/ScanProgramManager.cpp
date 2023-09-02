//
// Created by stefan on 11/20/22.
//

#include "ScanProgramManager.h"


namespace video::scanProgram {

        std::string ScanProgramManager::getTag() {
            return "ScanProgramManager";
        }

        void ScanProgramManager::swapTo(ScanProgram scanProgram) {
            mutex_enter_blocking(&this->scanProgramStateMutex);
            logger->d(getTag(), fmt::format("Swap to: {:x}. Previous: {:x}", (int)scanProgram, (int)previousScanProgram));
            this->previousScanProgram = this->currentScanProgram;
            this->currentScanProgram = scanProgram;

            if (currentScanProgram != previousScanProgram) {
                getScanProgramPtr(previousScanProgram)->stopScanProgram();
                getScanProgramPtr(currentScanProgram)->startScanProgram();
            }

            mutex_exit(&this->scanProgramStateMutex);
        }

        ScanProgram ScanProgramManager::getCurrentScanProgram() {
            ScanProgram ret;
            mutex_enter_blocking(&this->scanProgramStateMutex);
            ret = this->currentScanProgram;
            mutex_exit(&this->scanProgramStateMutex);
            logger->d(getTag(), fmt::format("Current: {:x}", (int)ret));
            return ret;
        }

        std::shared_ptr<scanPrograms::BaseScanProgram> ScanProgramManager::getScanProgramPtr(ScanProgram scanProgram) {
            std::shared_ptr<scanPrograms::BaseScanProgram> retPtr;
            switch (scanProgram) {
                case BOOT_SPLASH:
                    retPtr = std::reinterpret_pointer_cast<scanPrograms::BaseScanProgram>(this->bootsplashScanProgram);
                    break;
                case CLOCK:
                    retPtr = std::reinterpret_pointer_cast<scanPrograms::BaseScanProgram>(this->clockScanProgram);
                    break;
                case DEMO:
                    retPtr = std::reinterpret_pointer_cast<scanPrograms::BaseScanProgram>(this->demoScanProgram);
                    break;
                case MENU:
                    retPtr = std::reinterpret_pointer_cast<scanPrograms::BaseScanProgram>(this->menuScanProgram);
                    break;
                case NOOP:
                    retPtr = std::reinterpret_pointer_cast<scanPrograms::BaseScanProgram>(this->noopScanProgram);
                    break;
            }
            return retPtr;
        }

        void ScanProgramManager::cpu0setup() {
            if (classIsNoOp) { return; }
            getScanProgramPtr(getCurrentScanProgram())->cpu0setup();
        }

        void ScanProgramManager::cpu1setup() {
            if (classIsNoOp) { return; }
            getScanProgramPtr(getCurrentScanProgram())->cpu1Setup();
        }

        void ScanProgramManager::onCpu0Loop() {
            if (classIsNoOp) { return; }
            getScanProgramPtr(getCurrentScanProgram())->onCpu0Loop();
        }

        void ScanProgramManager::onCpu1Loop() {
            if (classIsNoOp) { return; }
            getScanProgramPtr(getCurrentScanProgram())->onCpu1Loop();
        }

        ScanProgramManager::ScanProgramManager(
                std::shared_ptr<pico::logger::BaseLogger> logger,
                std::shared_ptr<video::scanProgram::scanPrograms::noop::NoopScanProgram> noopScanProgram,
                std::shared_ptr<video::scanProgram::scanPrograms::menu::MenuScanProgram> menuScanProgram,
                std::shared_ptr<video::scanProgram::scanPrograms::demo::DemoScanProgram> demoScanProgram,
                std::shared_ptr<video::scanProgram::scanPrograms::clock::ClockScanProgram> clockScanProgram,
                std::shared_ptr<video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram> bootsplashScanProgram) {

            this->logger = logger;
            this->noopScanProgram = noopScanProgram;
            this->menuScanProgram = menuScanProgram;
            this->demoScanProgram = demoScanProgram;
            this->clockScanProgram = clockScanProgram;
        }
} // scanProgram