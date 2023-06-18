//
// Created by stefan on 6/18/23.
//

#include "HardPi4PowerOffMenuItem.h"

namespace video::ScreenManager::MainScreen {

    HardPi4PowerOffMenuItem::HardPi4PowerOffMenuItem(
            std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager) {
        this->pi4PowerSwitchManager = pi4PowerSwitchManager;
    }

    std::string HardPi4PowerOffMenuItem::getLabel() {
        return "Hard Pi4 power off";
    }

    void HardPi4PowerOffMenuItem::onItemClicked() {
        pi4PowerSwitchManager->setPower(false);
    }

} // MainScreen