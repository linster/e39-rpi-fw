//
// Created by stefan on 11/20/22.
//

#include "ScanProgramManager.h"


namespace video::scanProgram {

        std::string ScanProgramManager::getTag() {
            return "ScanProgramManager";
        }

        void ScanProgramManager::swapTo(ScanProgram scanProgram) {
            cpu0EnqueueSwapTo(scanProgram);
        }

        ScanProgram ScanProgramManager::getCurrentScanProgram() {
            ScanProgram ret;
            mutex_enter_blocking(&this->scanProgramStateMutex);
            ret = this->currentScanProgram;
            mutex_exit(&this->scanProgramStateMutex);
            //This runs on every CPU0 and CPU1 loop
            //logger->d(getTag(), fmt::format("Current: {:x}", (int)ret));
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
            //Set up all the scan programs...
            this->noopScanProgram->cpu0setup();
            this->menuScanProgram->cpu0setup();
            this->demoScanProgram->cpu0setup();
            this->clockScanProgram->cpu0setup();
        }

        void ScanProgramManager::cpu1setup() {
            if (classIsNoOp) { return; }

            measureFreqs();


            //TODO fix panic:
            //TODO panic("System clock (%d) must be an integer multiple of the requested pixel clock (%d).", sys_clk, timing->clock_freq);
            //TODO Sysclk is 125000000
            //TODO pixel clock is 7867500
            //https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#rpipf786bb684fa58b12b349
            //https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#rpipa610f0db2a24674346fd
//            //We need to set sys_clk to 118012500 (so that it is 15 times the pixel clock)
//
//            clock_configure(
//                    clk_sys,
//                    0,
//                    0,
//                    125 * MHZ,
//                    125 * MHZ
//            );
//
//            // Re init uart now that clk_peri has changed
//            stdio_init_all();
            measureFreqs();
            scanvideo_setup(&scanPrograms::BaseScanProgram::mode_bmbt);


            this->noopScanProgram->cpu1Setup();
            this->menuScanProgram->cpu1Setup();
            this->demoScanProgram->cpu1Setup();
            this->clockScanProgram->cpu1Setup();
        }

        void ScanProgramManager::onCpu0Loop() {
            if (classIsNoOp) { return; }
            getScanProgramPtr(getCurrentScanProgram())->onCpu0Loop();
        }

        void ScanProgramManager::onCpu1Loop() {
            if (classIsNoOp) { return; }
            cpu1DequeueSwapTo();
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
            this->bootsplashScanProgram = bootsplashScanProgram;

            mutex_init(&scanProgramStateMutex);
            queue_init(
                    &cpu1IncomingCommandQ,
                    sizeof(ScanProgram),
                    1
                    );
        }

    void ScanProgramManager::measureFreqs() {
        //Borrowed from https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#ga7ac25aa331f7c2624795b6088f87d133

        uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
        uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
        uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
        uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
        uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
        uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
        uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);
        uint f_clk_rtc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_RTC);

        logger->d(getTag(), fmt::format("pll_sys  = %d kHz\n", f_pll_sys));
        logger->d(getTag(), fmt::format("pll_usb  = %dkHz\n", f_pll_usb));
        logger->d(getTag(), fmt::format("rosc     = %dkHz\n", f_rosc));
        logger->d(getTag(), fmt::format("clk_sys  = %dkHz\n", f_clk_sys));
        logger->d(getTag(), fmt::format("clk_peri = %dkHz\n", f_clk_peri));
        logger->d(getTag(), fmt::format("clk_usb  = %dkHz\n", f_clk_usb));
        logger->d(getTag(), fmt::format("clk_adc  = %dkHz\n", f_clk_adc));
        logger->d(getTag(), fmt::format("clk_rtc  = %dkHz\n", f_clk_rtc));

        // Can't measure clk_ref / xosc as it is the ref

    }

    void ScanProgramManager::cpu0EnqueueSwapTo(ScanProgram scanProgram) {
        bool added = queue_try_add(&cpu1IncomingCommandQ, &scanProgram);
        if (!added) {
            logger->w(getTag(),
                      fmt::format("Could not add scanProgram {} to queue with size {}",
                                  (int)scanProgram,
                                  queue_get_level(&cpu1IncomingCommandQ)
                                  ));
        } else {
            logger->d(getTag(), "Enqueued request to swap scan program");
        }
    }

    void ScanProgramManager::cpu1DequeueSwapTo() {
        ScanProgram incoming;
        bool haveRequest = queue_try_remove(&cpu1IncomingCommandQ, &incoming);

        if (haveRequest) {
            //Do the swap
            logger->d(getTag(), "Have request to cpu1SwapTo");
            cpu1SwapTo(incoming);
        }
    }

    void ScanProgramManager::cpu1SwapTo(ScanProgram scanProgram) {
        mutex_enter_blocking(&this->scanProgramStateMutex);
        logger->d(getTag(), fmt::format("cpu1SwapTo Swap to: {:x}. Previous: {:x}", (int)scanProgram, (int)previousScanProgram));
        this->previousScanProgram = this->currentScanProgram;
        this->currentScanProgram = scanProgram;

        if (currentScanProgram != previousScanProgram) {
            //these need to be run from CPU1 because they setup scanvideo timing.
            getScanProgramPtr(previousScanProgram)->stopScanProgram();
            getScanProgramPtr(currentScanProgram)->startScanProgram();
        }

        mutex_exit(&this->scanProgramStateMutex);
    }

} // scanProgram