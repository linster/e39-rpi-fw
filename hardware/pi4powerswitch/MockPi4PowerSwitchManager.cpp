//
// Created by stefan on 11/21/22.
//

#include "MockPi4PowerSwitchManager.h"


namespace pico {
    namespace hardware {
        namespace pi4powerswitch {

            MockPi4PowerSwitchManager::MockPi4PowerSwitchManager(std::shared_ptr<logger::BaseLogger> baseLogger) {
                this->logger = baseLogger;
                logger->d("MockPi4PowerSwitchManager", "Constructor");
            }

            void MockPi4PowerSwitchManager::setPower(bool isOn) {
                logger->d("MockPi4PowerSwitchManager", fmt::format("Setting mock pi4 power pin to {}", (int)isOn));
            }
        } // pico
    } // hardware
} // pi4powerswitch