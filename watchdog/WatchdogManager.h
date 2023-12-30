//
// Created by stefan on 12/30/23.
//

#ifndef PICOTEMPLATE_WATCHDOGMANAGER_H
#define PICOTEMPLATE_WATCHDOGMANAGER_H


#include <memory>
#include <logging/BaseLogger.h>
#include "hardware/watchdog.h"

namespace watchdog {

    class WatchdogManager {

    private:
        const std::string TAG = "WatchdogManager";

        std::shared_ptr<pico::logger::BaseLogger> logger;

        /** Is the Watchdog timer enabled? */
        bool isEnabled = true;

        /** Does cpu0 contribute to clearing the watchdog timer?
         *  All of the IBUS logic happens on Cpu0, so this should contribute to keeping the system up
         */
        bool cpu0Contributes = true;

        /** Does cpu1 contribute to clearing the watchdog timer?
         * Only the video runs on Cpu1, but if CPU1 locks up, then the user loses the ability to
         * restart the rpi.
         */
        bool cpu1Contributes = false;

        void setupWatchdog();
        void tickWatchdog(uint8_t cpuNum);
    public:
        explicit WatchdogManager(
                std::shared_ptr<pico::logger::BaseLogger> logger
                );

        void cpu0setup();
        void onCpu0Loop();
        void cpu1Setup();
        void onCpu1Loop();
    };

} // watchdog

#endif //PICOTEMPLATE_WATCHDOGMANAGER_H
