//
// Created by stefan on 11/21/22.
//

#include "MockVideoSwitch.h"
#include "../../../libs/fmt/include/fmt/format.h"

namespace pico {
    namespace hardware {
        namespace videoSwitch {
            namespace mock {

                MockVideoSwitch::MockVideoSwitch(std::shared_ptr<logger::BaseLogger> baseLogger) {
                    logger = baseLogger;
                    logger->d("MockVideoSwitch", "Constructor.");
                    logger->d("MockVideoSwitch", "Setting previous video source to default to RVC.");
                    previousVideoSource = RVC;
                }
                void MockVideoSwitch::switchTo(pico::hardware::videoSwitch::VideoSource source) {
                    logger->d("MockVideoSwitch", "Request to change video switch");
                    logger->d("MockVideoSwitch", fmt::format("Previous value was: {}", previousVideoSource));
                    logger->d("MockVideoSwitch", fmt::format("New video switch value is {}", source));
                    previousVideoSource = source;
                }

                VideoSource MockVideoSwitch::getPreviousVideoSource() {
                    return previousVideoSource;
                }
            } // pico
        } // hardware
    } // videoSwitch
} // mock