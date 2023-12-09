//
// Created by stefan on 11/21/22.
//

#include "hardware/pio.h"
#include "Max4314VideoSwitch.h"


namespace pico {
    namespace hardware {
        namespace videoSwitch {
            namespace max4314 {

                Max4314VideoSwitch::Max4314VideoSwitch(std::shared_ptr<logger::BaseLogger> baseLogger) {
                    logger = baseLogger;
                    logger->d("Max4314VideoSwitch", "Constructor.");
                    logger->d("Max4314VideoSwitch", "Setting previous video source to default to RVC.");
                    previousVideoSource = RVC;

                    gpio_set_function(gpio_pin_lsb, GPIO_FUNC_SIO);
                    gpio_set_function(gpio_pin_msb, GPIO_FUNC_SIO);
                    //TODO use bit masks for this later.
                    gpio_set_dir(gpio_pin_lsb, true);
                    gpio_set_dir(gpio_pin_msb, true);

                    switchTo(RVC);
                }

                void Max4314VideoSwitch::switchTo(pico::hardware::videoSwitch::VideoSource source) {
                    switch (source) {
                        case VideoSource::RVC:
                            logger->d("Max4314VideoSwitch", "Setting video source to RVC");
                            gpio_put(gpio_pin_msb, false);
                            gpio_put(gpio_pin_lsb, false);
                            break;
                        case VideoSource::UPSTREAM:
                            logger->d("Max4314VideoSwitch", "Setting video source to UPSTREAM");
                            gpio_put(gpio_pin_msb, false);
                            gpio_put(gpio_pin_lsb, true);
                            break;
                        case VideoSource::PICO:
                            logger->d("Max4314VideoSwitch", "Setting video source to PICO");
                            gpio_put(gpio_pin_msb, true);
                            gpio_put(gpio_pin_lsb, false);
                            break;
                        case VideoSource::PI:
                            logger->d("Max4314VideoSwitch", "Setting video source to PI");
                            gpio_put(gpio_pin_msb, true);
                            gpio_put(gpio_pin_lsb, true);
                            break;
                        default:
                            logger->w("Max4314VideoSwitch", "Invalid video source requested.");
                    }
                    previousVideoSource = source;
                }

                VideoSource Max4314VideoSwitch::getPreviousVideoSource() {
                    switch (previousVideoSource) {
                        case RVC:
                            logger->d("Max4314VideoSwitch", "Returning previous video source: RVC");
                            break;
                        case UPSTREAM:
                            logger->d("Max4314VideoSwitch", "Returning previous video source: UPSTREAM");
                            break;
                        case PICO:
                            logger->d("Max4314VideoSwitch", "Returning previous video source: PICO");
                            break;
                        case PI:
                            logger->d("Max4314VideoSwitch", "Returning previous video source: PI");
                            break;
                    }
                    return previousVideoSource;
                }

            } // pico
        } // hardware
    } // videoSwitch
} // max4314