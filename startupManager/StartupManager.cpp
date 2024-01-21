//
// Created by stefan on 1/20/24.
//

#include "StartupManager.h"

namespace pico::startupManager {
    StartupManager::StartupManager(std::shared_ptr <ApplicationContainer> applicationContainer) {
        this->applicationContainer = applicationContainer;
    }

    void StartupManager::cpu0main() {

        applicationContainer->onMain();

        multicore_reset_core1();


    }

    void StartupManager::core1Func() {

    }
} // startupManager