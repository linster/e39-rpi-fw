//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_IPI4POWERSWITCHMANAGER_H
#define PICOTEMPLATE_IPI4POWERSWITCHMANAGER_H

namespace pico {
    namespace hardware {
        namespace pi4powerswitch {

            class IPi4PowerSwitchManager {
            public:
                virtual void setPower(bool isOn) = 0;
            };

        } // pico
    } // hardware
} // pi4powerswitch

#endif //PICOTEMPLATE_IPI4POWERSWITCHMANAGER_H
