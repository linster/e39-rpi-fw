//
// Created by stefan on 12/30/23.
//

#include "WatchdogManager.h"

namespace watchdog {
    WatchdogManager::WatchdogManager(
            std::shared_ptr<pico::logger::BaseLogger> logger
    ) {
        this->logger = logger;
    }

    void WatchdogManager::cpu0setup() {
        setupWatchdog();
    }

    void WatchdogManager::cpu1Setup() {

    }

    void WatchdogManager::onCpu0Loop() {
        if (cpu0Contributes) {
            tickWatchdog(0);
        }
    }

    void WatchdogManager::onCpu1Loop() {
        if (cpu1Contributes) {
            tickWatchdog(1);
        }
    }

    void WatchdogManager::setupWatchdog() {
        if (!isEnabled) {
            logger->i(TAG, "Watchdog is not enabled. Continuing normal boot.");
            return;
        }

        if (watchdog_caused_reboot()) {
            logger->e(TAG, "watchdog_caused_reboot");
        }

        //Reboot after 5 seconds. Pretty conservative estimate
        //for non-responsiveness.
        watchdog_enable(5000, true);
    }

    void WatchdogManager::tickWatchdog(uint8_t cpuNum) {
        watchdog_update();
    }
} // watchdog