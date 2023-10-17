//
// Created by stefan on 11/21/22.
//


#include "GpioPi4PowerSwitchManager.h"


namespace pico::hardware::pi4powerswitch {

            GpioPi4PowerSwitchManager::GpioPi4PowerSwitchManager(std::shared_ptr<logger::BaseLogger> baseLogger) {
                this->logger = baseLogger;

                logger->d("GpioPi4PowerSwitchManager", "Constructor");

                gpio_set_function(gpio_pin, GPIO_FUNC_SIO);

                gpio_init(gpio_pin);
                gpio_set_dir(gpio_pin, GPIO_OUT);
                gpio_pull_down(gpio_pin);
                gpio_put(gpio_pin, false);

            }
            void GpioPi4PowerSwitchManager::setPower(bool isOn) {
                logger->d("GpioPi4PowerSwitchManager", fmt::format("Setting {} pin to {}", gpio_pin, isOn));
                gpio_put(gpio_pin, isOn);
            }

} // pi4powerswitch