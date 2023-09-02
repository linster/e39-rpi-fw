//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_SCANPROGRAMMANAGER_H
#define PICOTEMPLATE_SCANPROGRAMMANAGER_H

#include <memory>
#include "pico/mutex.h"
#include "ScanProgram.h"

#include "scanPrograms/BaseScanProgram.h"
#include "scanPrograms/noop/NoopScanProgram.h"
#include "scanPrograms/menu/MenuScanProgram.h"
#include "scanPrograms/demo/DemoScanProgram.h"
#include "scanPrograms/clock/ClockScanProgram.h"
#include "scanPrograms/bootsplash/BootsplashScanProgram.h"

#include "../../logging/BaseLogger.h"
#include "fmt/format.h"

namespace video::scanProgram {

        class ScanProgramManager {


        private:

            std::shared_ptr<pico::logger::BaseLogger> logger;

            std::shared_ptr<video::scanProgram::scanPrograms::noop::NoopScanProgram> noopScanProgram;
            std::shared_ptr<video::scanProgram::scanPrograms::menu::MenuScanProgram> menuScanProgram;
            std::shared_ptr<video::scanProgram::scanPrograms::demo::DemoScanProgram> demoScanProgram;
            std::shared_ptr<video::scanProgram::scanPrograms::clock::ClockScanProgram> clockScanProgram;
            std::shared_ptr<video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram> bootsplashScanProgram;

            //True to make all the running hooks do nothing.
            bool classIsNoOp = true;

            static std::string getTag();

            /** Guard the currently running scan program */
            mutex_t scanProgramStateMutex;
            //Guard: scanProgramStateMutex
            ScanProgram previousScanProgram = NOOP;
            ScanProgram currentScanProgram = NOOP;
            //End Guard: scanProgramStateMutex

            std::shared_ptr<scanPrograms::BaseScanProgram> getScanProgramPtr(ScanProgram scanProgram);



        public:

            ScanProgramManager(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<video::scanProgram::scanPrograms::noop::NoopScanProgram> noopScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::menu::MenuScanProgram> menuScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::demo::DemoScanProgram> demoScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::clock::ClockScanProgram> clockScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram> bootsplashScanProgram,
            );

            //Called only by ApplicationContainer
            void cpu0setup();
            void cpu1setup();
            void onCpu0Loop();
            void onCpu1Loop();

            //TODO on what CPU core was I going to run the scan program on?
            //TODO all the lin logic is on cpu0
            //TODO all the IRQs and shuffling crap is on cpu1
            //TODO if I put the scan program on 0 it won't interfere with the bad non-dma?

            //TODO it doesn't matter which CPU. onCpu0Loop gets called, we then delagate to the
            //TODO currently running scan program. This is all a big game of non-blocking IO. Don't
            //TODO put in while(true) in the scan program, just let the application container loop
            //TODO deal with actually looping. Then, if cpu0 is so slow we can't keep a screen
            //TODO filled, move it to cpu1.

            //These are only called from the ScanProgramSwapper.
            void swapTo(ScanProgram scanProgram);
            ScanProgram getCurrentScanProgram();



        };



    } // scanProgram

#endif //PICOTEMPLATE_SCANPROGRAMMANAGER_H
