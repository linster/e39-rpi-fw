//
// Created by stefan on 6/18/23.
//

#include "RestartXMenuItem.h"

namespace video::ScreenManager::MainScreen {

            RestartXMenuItem::RestartXMenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter) {
                this->logger = logger;
                this->softPowerRequestWriter = softPowerRequestWriter;
                if (this->softPowerRequestWriter == nullptr) {
                    logger->wtf("RestartXMenuItem", "softPowerRequestWriter is null");
                }
            }

            void RestartXMenuItem::onItemClicked() {
                softPowerRequestWriter->requestRpiRestartX();
            }

            std::string RestartXMenuItem::getLabel() {
                return "Restart X";
            }

} // MainScreen