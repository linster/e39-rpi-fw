//
// Created by stefan on 11/21/22.
//

#include "pico.h"
#include "hardware/pio.h"
#include "Max4314VideoSwitch.h"
#include "../../../libs/fmt/include/fmt/format.h"

namespace pico {
    namespace hardware {
        namespace videoSwitch {
            namespace max4314 {

                Max4314VideoSwitch::Max4314VideoSwitch(std::shared_ptr<logger::BaseLogger> baseLogger) {
                    logger = baseLogger;
                    logger->d("Max4314VideoSwitch", "Constructor.");
                    logger->d("Max4314VideoSwitch", "Setting previous video source to default to RVC.");
                    previousVideoSource = RVC;

                    //TODO use bit masks for this later.
                    gpio_set_dir(gpio_pin_lsb, true);
                    gpio_set_dir(gpio_pin_msb, true);
                    gpio_pull_down(gpio_pin_lsb);
                    gpio_pull_down(gpio_pin_msb);
                }

                void Max4314VideoSwitch::switchTo(pico::hardware::videoSwitch::VideoSource source) {
                    logger->d("Max4314VideoSwitch", fmt::format("Setting video source to %i", (int)source));
                    switch (source) {
                        case VideoSource::RVC:
                            gpio_put(gpio_pin_msb, false);
                            gpio_put(gpio_pin_lsb, false);
                            break;
                        case VideoSource::UPSTREAM:
                            gpio_put(gpio_pin_msb, false);
                            gpio_put(gpio_pin_lsb, true);
                            break;
                        case VideoSource::PICO:
                            gpio_put(gpio_pin_msb, true);
                            gpio_put(gpio_pin_lsb, false);
                            break;
                        case VideoSource::PI:
                            gpio_put(gpio_pin_msb, true);
                            gpio_put(gpio_pin_lsb, true);
                            break;
                        default:
                            logger->w("Max4314VideoSwitch", "Invalid video source requested.");
                    }
                    previousVideoSource = source;
                }

                VideoSource Max4314VideoSwitch::getPreviousVideoSource() {
                    logger->d("Max4314VideoSwitch", fmt::format("Returning previous video source: %i", (int)previousVideoSource));
                    return previousVideoSource;
                }

            } // pico
        } // hardware
    } // videoSwitch
} // max4314