//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_IBUSDEVICEENUM_H
#define PICOTEMPLATE_IBUSDEVICEENUM_H

#include "IBusDevices.h"

#include <string>

namespace pico {
    namespace ibus {
        namespace data {

            enum IbusDeviceEnum {
                BODY_MODULE = BODY_MODULE_VALUE,
                BROADCAST = BROADCAST_VALUE,
                MFL = MFL_VALUE,
                RADIO = RADIO_VALUE,
                MID = MID_VALUE,
                TELEPHONE = TELEPHONE_VALUE,
                IKE = IKE_VALUE,
                IKE_TEXTBAR = IKE_TEXTBAR_VALUE,
                TV_MODULE = TV_MODULE_VALUE,
                NAV_VIDEOMODULE = NAV_VIDEOMODULE_VALUE,
                NAV_MENUSCREEN = NAV_MENUSCREEN_VALUE,
                NAV_LOCATION = NAV_LOCATION_VALUE,
                BOARDMONITOR_BUTTONS = BOARDMONITOR_BUTTONS_VALUE,
                DIS = DIS_VALUE,

                NAVJ = NAVJ_VALUE,
                //We're going to use NAVJ for our Pico<->Pi subnet, because no real person still has an e39
                //with NAVJ (Japan-only Navigation)
                PICO = NAVJ_VALUE,

                //We use MINI (D-Bus identifier) for PI4 -> PICO comms. On an e39
                //there should be no device that responds to this dest address.
                //https://github.com/piersholt/wilhelm-docs#ki-bus
                PI4 = 0x31
            };

        } // pico
    } // ibus
} // data

#endif //PICOTEMPLATE_IBUSDEVICEENUM_H
