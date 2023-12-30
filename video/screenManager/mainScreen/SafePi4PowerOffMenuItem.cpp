//
// Created by stefan on 6/18/23.
//

#include "SafePi4PowerOffMenuItem.h"

namespace video::ScreenManager::MainScreen {

            SafePi4PowerOffMenuItem::SafePi4PowerOffMenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter
                    ) {
                this->logger = logger;
                this->softPowerRequestWriter = softPowerRequestWriter;
                if (this->softPowerRequestWriter == nullptr) {
                    logger->wtf("SafePi4PowerOffMenuItem", "softPowerRequestWriter is null");
                }
            }

            void SafePi4PowerOffMenuItem::onItemClicked() {
                softPowerRequestWriter->requestRpiRestart();
            }

            std::string SafePi4PowerOffMenuItem::getLabel() {
                return "Safe Pi4 reboot";
            }
        } // MainScreen