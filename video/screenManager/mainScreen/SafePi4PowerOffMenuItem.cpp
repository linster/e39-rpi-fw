//
// Created by stefan on 6/18/23.
//

#include "SafePi4PowerOffMenuItem.h"

namespace video::ScreenManager::MainScreen {

            SafePi4PowerOffMenuItem::SafePi4PowerOffMenuItem(
                    std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter
                    ) {
                this->softPowerRequestWriter = softPowerRequestWriter;
            }

            void SafePi4PowerOffMenuItem::onItemClicked() {
                softPowerRequestWriter->requestRpiRestart();
            }

            std::string SafePi4PowerOffMenuItem::getLabel() {
                return "Safe Pi4 reboot";
            }
        } // MainScreen