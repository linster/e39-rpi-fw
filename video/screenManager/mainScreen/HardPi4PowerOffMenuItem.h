//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_HARDPI4POWEROFFMENUITEM_H
#define PICOTEMPLATE_HARDPI4POWEROFFMENUITEM_H

#include <memory>
#include "../../../hardware/pi4powerswitch/IPi4PowerSwitchManager.h"
#include "../ScreenItem.h"

namespace video {
    namespace ScreenManager {
        namespace MainScreen {

            class HardPi4PowerOffMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;

            public:
                HardPi4PowerOffMenuItem(
                        std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager
                        );

                void onItemClicked() override;
                std::string getLabel() override;
            };

        } // video
    } // ScreenManager
} // MainScreen

#endif //PICOTEMPLATE_HARDPI4POWEROFFMENUITEM_H
