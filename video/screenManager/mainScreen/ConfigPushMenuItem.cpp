//
// Created by stefan on 6/18/23.
//

#include "ConfigPushMenuItem.h"

namespace video {
    namespace ScreenManager {
        namespace MainScreen {

            ConfigPushMenuItem::ConfigPushMenuItem(
                    std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                    std::shared_ptr<pico::config::ConfigurationManager> configurationManager) {
                this->configurationStatusWriter = configurationStatusWriter;
                this->configurationManager = configurationManager;
            }

            std::string ConfigPushMenuItem::getLabel() {
                return "Config Push (PicoToPi)";
            }

            void ConfigPushMenuItem::onItemClicked() {
                configurationStatusWriter->scheduleEmit(
                        configurationManager->getConfigurationCopy().toMessage());
            }
        } // video
    } // ScreenManager
} // MainScreen