//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_SCANPROGRAMMANAGER_H
#define PICOTEMPLATE_SCANPROGRAMMANAGER_H

#include <memory>
#include <cstdint>
#include "pico/stdio.h"
#include "pico/mutex.h"
#include "ScanProgram.h"

#include <video/scanProgram/scanPrograms/BaseScanProgram.h>
#include <video/scanProgram/scanPrograms/noop/NoopScanProgram.h>
#include <video/scanProgram/scanPrograms/menu/MenuScanProgram.h>
#include <video/scanProgram/scanPrograms/demo/DemoScanProgram.h>
#include <video/scanProgram/scanPrograms/clock/ClockScanProgram.h>
#include <video/scanProgram/scanPrograms/bootsplash/BootsplashScanProgram.h>

#include <logging/BaseLogger.h>
#include "fmt/format.h"

#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"
#include "pico/util/queue.h"

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
            bool classIsNoOp = false;

            static std::string getTag();

            /** Guard the currently running scan program */
            mutex_t scanProgramStateMutex;
            //Guard: scanProgramStateMutex
            ScanProgram previousScanProgram = NOOP;
            ScanProgram currentScanProgram = NOOP;
            //End Guard: scanProgramStateMutex

            std::shared_ptr<scanPrograms::BaseScanProgram> getScanProgramPtr(ScanProgram scanProgram);
            void measureFreqs();

            /** CPU_0 -> CPU_1 command queue */
            queue_t cpu1IncomingCommandQ;
            void cpu0EnqueueSwapTo(ScanProgram scanProgram);
            void cpu1DequeueSwapTo();
            void cpu1SwapTo(ScanProgram scanProgram);

        public:

            ScanProgramManager(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<video::scanProgram::scanPrograms::noop::NoopScanProgram> noopScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::menu::MenuScanProgram> menuScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::demo::DemoScanProgram> demoScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::clock::ClockScanProgram> clockScanProgram,
                    std::shared_ptr<video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram> bootsplashScanProgram
            );

            //Called only by ApplicationContainer
            void cpu0setup();
            void cpu1setup();
            void onCpu0Loop();
            void onCpu1Loop();

            //These are only called from the ScanProgramSwapper, which is called from CPU0.s
            void swapTo(ScanProgram scanProgram);
            ScanProgram getCurrentScanProgram();



        };



    } // scanProgram

#endif //PICOTEMPLATE_SCANPROGRAMMANAGER_H
