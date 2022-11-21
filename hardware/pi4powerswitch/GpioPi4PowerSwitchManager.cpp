//
// Created by stefan on 11/21/22.
//

#include "pico.h"
#include "hardware/pio.h"

#include "GpioPi4PowerSwitchManager.h"

namespace pico {
    namespace hardware {
        namespace pi4powerswitch {

            GpioPi4PowerSwitchManager::GpioPi4PowerSwitchManager(std::shared_ptr<logger::BaseLogger> baseLogger) {
                this->logger = baseLogger;

                logger->d("GpioPi4PowerSwitchManager", "Constructor");

                gpio_init(gpio_pin);
                gpio_set_dir(gpio_pin, true);
                gpio_pull_down(gpio_pin);

            }
            void GpioPi4PowerSwitchManager::setPower(bool isOn) {

                logger->d("GpioPi4PowerSwitchManager", std::sprintf()
                          reinterpret_cast<const char *>(sprintf("Setting %d pin to %b",
                                                                 reinterpret_cast<const char *>(gpio_pin), isOn)));

                gpio_put(gpio_pin, isOn);
            }

        } // pico
    } // hardware
} // pi4powerswitch