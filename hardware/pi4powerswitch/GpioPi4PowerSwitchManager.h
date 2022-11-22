//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_GPIOPI4POWERSWITCHMANAGER_H
#define PICOTEMPLATE_GPIOPI4POWERSWITCHMANAGER_H

#include <memory>
#include "../../logging/BaseLogger.h"
#include "IPi4PowerSwitchManager.h"

namespace pico {
    namespace hardware {
        namespace pi4powerswitch {

            class GpioPi4PowerSwitchManager : public IPi4PowerSwitchManager {

            private:
                std::shared_ptr<logger::BaseLogger> logger;
                int gpio_pin = 18;
            public:
                GpioPi4PowerSwitchManager(
                        std::shared_ptr<logger::BaseLogger> baseLogger
                );
                void setPower(bool isOn) override;
            };

        } // pico
    } // hardware
} // pi4powerswitch

#endif //PICOTEMPLATE_GPIOPI4POWERSWITCHMANAGER_H
