//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_TURNONPI4MENUITEM_H
#define PICOTEMPLATE_TURNONPI4MENUITEM_H

#include <memory>
#include "../../../hardware/pi4powerswitch/IPi4PowerSwitchManager.h"
#include "../../../logging/BaseLogger.h"
#include "../ScreenItem.h"

namespace video::ScreenManager::MainScreen {

            class TurnOnPi4MenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::logger::BaseLogger> logger;
                std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;

            public:

                TurnOnPi4MenuItem(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager
                        );

                std::string getLabel() override;
                void onItemClicked() override;
            };

        } // MainScreen

#endif //PICOTEMPLATE_TURNONPI4MENUITEM_H
