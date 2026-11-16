//
// Created by stefan on 11/20/22.
//

#include "ScanProgramManager.h"
#include "hardware/timer.h"
#include "hardware/pio.h"


namespace video::scanProgram {

        std::string ScanProgramManager::getTag() {
            return "ScanProgramManager";
        }

        void ScanProgramManager::swapTo(ScanProgram scanProgram) {
            cpu0EnqueueSwapTo(scanProgram);
//            cpu1SwapTo(scanProgram);
//            cpu0SwapTo(scanProgram);
        }

        void ScanProgramManager::swapToBootProgram(pico::messages::ConfigMessage::ScanProgram configured) {
            using Configured = pico::messages::ConfigMessage::ScanProgram;
            using SplashType = scanPrograms::bootsplash::BootsplashScanProgram::BootSplashType;
            switch (configured) {
                case Configured::NONE:
                    swapTo(ScanProgram::NOOP);
                    return;
                case Configured::CLOCK:
                    swapTo(ScanProgram::CLOCK);
                    return;
                case Configured::MENU:
                    swapTo(ScanProgram::MENU);
                    return;
                case Configured::GOOSE_BOOTSPLASH:
                    bootsplashScanProgram->setBootSplashType(SplashType::GOOSE);
                    break;
                case Configured::BMW_BOOTSPLASH:
                    bootsplashScanProgram->setBootSplashType(SplashType::BMW);
                    break;
                case Configured::LINSTEROS_BOOTSPLASH:
                default: //A value this firmware doesn't know (newer config): show the default splash.
                    bootsplashScanProgram->setBootSplashType(SplashType::LINSTER_OS);
                    break;
            }
            swapTo(ScanProgram::BOOT_SPLASH);
        }

        ScanProgram ScanProgramManager::getCurrentScanProgram() {
            ScanProgram ret;
//            mutex_enter_blocking(&this->scanProgramStateMutex);
            ret = this->currentScanProgram;
//            mutex_exit(&this->scanProgramStateMutex);
            //This runs on every CPU0 and CPU1 loop
            //logger->d(getTag(), fmt::format("Current: {:x}", (int)ret));
            return ret;
        }

        //Returns a raw pointer on purpose. This is called on both cores, on every loop iteration
        //(core 0) and every scanline (core 1). Returning a std::shared_ptr copy bumped the shared
        //reference count from both cores at once, and on this toolchain shared_ptr reference counts
        //aren't atomic (_Lock_policy _S_single). Lost updates eventually dropped the count to zero,
        //deleting the scan program while it was in use: core 1 then called render() through the
        //destroyed object's BaseScanProgram vtable and jumped to address 0 (HardFault).
        //The ScanProgramManager owns all scan programs for the life of the program, so no
        //reference counting is needed here.
        scanPrograms::BaseScanProgram* ScanProgramManager::getScanProgramPtr(ScanProgram scanProgram) {
            switch (scanProgram) {
                case BOOT_SPLASH:
                    return this->bootsplashScanProgram.get();
                case CLOCK:
                    return this->clockScanProgram.get();
                case DEMO:
                    return this->demoScanProgram.get();
                case MENU:
                    return this->menuScanProgram.get();
                case NOOP:
                    return this->noopScanProgram.get();
            }
            return this->noopScanProgram.get();
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

// TODO STEFAN need to fiddle with the timing so we keep clk_sys at 125001000 so USB works,
// TODO but then set the pixel clock to 8333400, 15 times slower, which is 1.059218303
// 125000000÷(7867500×0.3)
//what if we 1/3rd the pixel clock and then multiply all the times by 3?
//that'll get us from 99.3% right timing to 99.9254% timing which might work long enough before we lose signal?
//
//
//            [21:52:29:494] *** PANIC ***␍␊
//            [21:52:29:494] ␍␊
//            [21:52:29:494] System clock (125000000) must be an integer multiple of the requested pixel clock (7867500).␍␊

//
//            [21:59:41:080] [INFO]    [onCpu1Main]:[scanProgramManager->cpu1setup()] ␍␊
//            [21:59:41:095] [DEBUG]    [ScanProgramManager]:[pll_sys  = 125001 kHz] ␍␊
//            [21:59:41:095] [DEBUG]    [ScanProgramManager]:[pll_usb  = 48000 kHz] ␍␊
//            [21:59:41:095] [DEBUG]    [ScanProgramManager]:[rosc     = 5490 kHz] ␍␊
//            [21:59:41:117] [DEBUG]    [ScanProgramManager]:[clk_sys  = 125001 kHz] ␍␊
//            [21:59:41:117] [DEBUG]    [ScanProgramManager]:[clk_peri = 125000 kHz] ␍␊
//            [21:59:41:117] [DEBUG]    [ScanProgramManager]:[clk_usb  = 48000 kHz] ␍␊
//            [21:59:41:117] [DEBUG]    [ScanProgramManager]:[clk_adc  = 48000 kHz] ␍␊
//            [21:59:41:133] [DEBUG]    [ScanProgramManager]:[clk_rtc  = 46 kHz] ␍␊
//            [21:59:41:133] [DEBUG]    [ScanProgramManager]:[pll_sys  = 125000 kHz] ␍␊
//            [21:59:41:144] [DEBUG]    [ScanProgramManager]:[pll_usb  = 48000 kHz] ␍␊
//            [21:59:41:144] [DEBUG]    [ScanProgramManager]:[rosc     = 5485 kHz] ␍␊
//            [21:59:41:144] [DEBUG]    [ScanProgramManager]:[clk_sys  = 125000 kHz] ␍␊
//            [21:59:41:156] [DEBUG]    [ScanProgramManager]:[clk_peri = 125000 kHz] ␍␊
//            [21:59:41:156] [DEBUG]    [ScanProgramManager]:[clk_usb  = 48000 kHz] ␍␊
//            [21:59:41:166] [DEBUG]    [ScanProgramManager]:[clk_adc  = 48000 kHz] ␍␊
//            [21:59:41:166] [DEBUG]    [ScanProgramManager]:[clk_rtc  = 47 kHz] ␍␊
//            [21:59:41:175] ␍␊
//TODO we might actually have to change this.


            measureFreqs();



            scanvideo_setup(&scanPrograms::BaseScanProgram::mode_bmbt);
            //We are never going to disable the scanvideo timing, even when in noop,
            //we'll just keep the state machine running.
            sleep_ms(40);
            scanvideo_timing_enable(true);

            sleep_ms(500);

            this->noopScanProgram->cpu1Setup();
            this->menuScanProgram->cpu1Setup();
            this->demoScanProgram->cpu1Setup();
            this->clockScanProgram->cpu1Setup();
        }

        void ScanProgramManager::onCpu0Loop() {
            //No scan program should be doing anything on cpu0Loop.
            if (classIsNoOp) { return; }
            recoverWedgedScanlineStateMachine();
            //Let scan programs do their core 0 half of starting/stopping (see BaseScanProgram).
            for (auto program : {(scanPrograms::BaseScanProgram*) noopScanProgram.get(),
                                 (scanPrograms::BaseScanProgram*) menuScanProgram.get(),
                                 (scanPrograms::BaseScanProgram*) demoScanProgram.get(),
                                 (scanPrograms::BaseScanProgram*) clockScanProgram.get(),
                                 (scanPrograms::BaseScanProgram*) bootsplashScanProgram.get()}) {
                program->serviceCpu0();
            }
            getScanProgramPtr(getCurrentScanProgram())->onCpu0Loop();
        }

        //Workaround: on some boots, scanvideo's scanline state machine (PIO0 SM0) wedges at its
        //line-start `wait 1 irq 4`: it never proceeds even though the timing state machine keeps
        //raising IRQ 4 and its TX FIFO holds the next line. Sync keeps running but the picture
        //freezes (blank screen), and core 1 then blocks forever in
        //scanvideo_begin_scanline_generation() because no buffer is ever consumed - which is why
        //this check runs on core 0. A state machine restart (SM_RESTART) clears it, after which
        //video stays up. Root cause not yet found: it's a boot-time race, not caused by rendering
        //or by flash writes (seen with the persistent config sector erased and never written).
        //
        //Detection: the SM is at that wait while IRQ flag 4 is already set. A running SM takes a set
        //flag within one PIO clock (8 system clocks here), so seeing both on checks 1 ms apart means
        //it's stuck. This used to count how often core 0 happened to see the SM at the wait
        //(>= 200 times over > 50 ms) instead. A healthy SM spends ~30% of every line there, so it
        //couldn't tell a stuck SM from a slow or unlucky core 0 loop: it fired on about half of all
        //scan program switches and restarted a working state machine mid-frame (a visible glitch).
        void ScanProgramManager::recoverWedgedScanlineStateMachine() {
            constexpr uint scanlineSm = 0;                  //PICO_SCANVIDEO_SCANLINE_SM (scanvideo uses pio0)
            constexpr uint lineStartIrq = 4;
            constexpr uint32_t confirmAfterUs = 1000;
            static const uint32_t lineStartWait = pio_encode_wait_irq(true, false, lineStartIrq);

            bool stuck = pio0->sm[scanlineSm].instr == lineStartWait && (pio0->irq & (1u << lineStartIrq));
            uint32_t now = time_us_32();
            if (!stuck) {
                scanlineSmStuckSinceUs = 0;
                return;
            }
            if (scanlineSmStuckSinceUs == 0) {
                scanlineSmStuckSinceUs = now | 1; //0 means "not stuck"
                return;
            }
            if ((now - scanlineSmStuckSinceUs) < confirmAfterUs) {
                return;
            }

            uint addr = pio0->sm[scanlineSm].addr;
            pio_sm_restart(pio0, scanlineSm);
            pio_sm_exec(pio0, scanlineSm, pio_encode_jmp(addr));
            scanlineSmStuckSinceUs = 0;
            scanlineSmRecoveries = scanlineSmRecoveries + 1;
            logger->w(getTag(), fmt::format("Restarted wedged scanline state machine ({} so far)",
                                            (uint32_t) scanlineSmRecoveries));
        }

        void ScanProgramManager::onCpu1Loop() {
            if (classIsNoOp) { return; }
            if (benchCore1StallMs != 0) {
                uint8_t ms = benchCore1StallMs;
                benchCore1StallMs = 0;
                busy_wait_ms(ms);
            }
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

        logger->d(getTag(), fmt::format("pll_sys  = {:d} kHz", f_pll_sys));
        logger->d(getTag(), fmt::format("pll_usb  = {:d} kHz", f_pll_usb));
        logger->d(getTag(), fmt::format("rosc     = {:d} kHz", f_rosc));
        logger->d(getTag(), fmt::format("clk_sys  = {:d} kHz", f_clk_sys));
        logger->d(getTag(), fmt::format("clk_peri = {:d} kHz", f_clk_peri));
        logger->d(getTag(), fmt::format("clk_usb  = {:d} kHz", f_clk_usb));
        logger->d(getTag(), fmt::format("clk_adc  = {:d} kHz", f_clk_adc));
        logger->d(getTag(), fmt::format("clk_rtc  = {:d} kHz", f_clk_rtc));

        // Can't measure clk_ref / xosc as it is the ref

    }

    void ScanProgramManager::cpu0EnqueueSwapTo(ScanProgram scanProgram) {
        bool added = queue_try_add(&cpu1IncomingCommandQ, &scanProgram);
        if (!added) {
            logger->wtf(getTag(), "Could not add scanProgram swap request to q. Next statement might cause a fmt crash.");
            logger->w(getTag(),
                      //TODO this might be the fmt statement that blows up and crashes a cpu core?
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
        swapTo(1, scanProgram);
    }

    void ScanProgramManager::cpu0SwapTo(ScanProgram scanProgram) {
        swapTo(0, scanProgram);
    }

    void ScanProgramManager::swapTo(uint8_t cpuNum, ScanProgram scanProgram) {
//        mutex_enter_blocking(&this->scanProgramStateMutex);
        logger->d(getTag(), fmt::format("cpu{:x}SwapTo Swap to: {:x}. Previous: {:x}", cpuNum, (int)scanProgram, (int)currentScanProgram));

        ScanProgram local_previousScanProgram = this->currentScanProgram;
        this->currentScanProgram = scanProgram;
        bool shouldSwap = (currentScanProgram != local_previousScanProgram);

        if (shouldSwap) {
            //these need to be run from CPU1 because they setup scanvideo timing.
            //TODO this might not be needed here?
            scanvideo_wait_for_vblank();
            getScanProgramPtr(local_previousScanProgram)->stopScanProgram();
            getScanProgramPtr(currentScanProgram)->startScanProgram();
        }

//        mutex_exit(&this->scanProgramStateMutex);
    }

} // scanProgram