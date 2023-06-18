//
// Created by stefan on 6/18/23.
//

#include "TurnOnPi4MenuItem.h"

namespace video::ScreenManager::MainScreen {

            TurnOnPi4MenuItem::TurnOnPi4MenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager) {
                this->logger = logger;
                this->pi4PowerSwitchManager = pi4PowerSwitchManager;
            }

            std::string TurnOnPi4MenuItem::getLabel() {
                return "Turn on Pi";
            }

            void TurnOnPi4MenuItem::onItemClicked() {
                this->pi4PowerSwitchManager->setPower(true);
            }

        } // MainScreen