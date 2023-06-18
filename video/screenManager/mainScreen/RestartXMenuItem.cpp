//
// Created by stefan on 6/18/23.
//

#include "RestartXMenuItem.h"

namespace video::ScreenManager::MainScreen {

            RestartXMenuItem::RestartXMenuItem(
                    std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter) {
                this->softPowerRequestWriter = softPowerRequestWriter;
            }

            void RestartXMenuItem::onItemClicked() {
                softPowerRequestWriter->requestRpiRestartX();
            }

            std::string RestartXMenuItem::getLabel() {
                return "Restart X";
            }

} // MainScreen