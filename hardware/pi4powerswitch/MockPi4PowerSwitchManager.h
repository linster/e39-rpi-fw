//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_MOCKPI4POWERSWITCHMANAGER_H
#define PICOTEMPLATE_MOCKPI4POWERSWITCHMANAGER_H

#include <memory>
#include "IPi4PowerSwitchManager.h"
#include "../../logging/BaseLogger.h"

namespace pico {
    namespace hardware {
        namespace pi4powerswitch {

            class MockPi4PowerSwitchManager : public IPi4PowerSwitchManager {

                MockPi4PowerSwitchManager(
                        std::shared_ptr<logger::BaseLogger> baseLogger
                );

            private:
                std::shared_ptr<logger::BaseLogger> logger;

            public:
                void setPower(bool isOn) override;
            };

        } // pico
    } // hardware
} // pi4powerswitch

#endif //PICOTEMPLATE_MOCKPI4POWERSWITCHMANAGER_H
