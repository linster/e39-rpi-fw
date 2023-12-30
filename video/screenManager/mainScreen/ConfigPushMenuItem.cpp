//
// Created by stefan on 6/18/23.
//

#include "ConfigPushMenuItem.h"

namespace video::ScreenManager::MainScreen {

            ConfigPushMenuItem::ConfigPushMenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                    std::shared_ptr<pico::config::ConfigurationManager> configurationManager) {

                this->logger = logger;
                this->configurationStatusWriter = configurationStatusWriter;
                if (this->configurationStatusWriter == nullptr) {
                    logger->wtf("ConfigPushMenuItem", "configurationStatusWriter is null");
                }
                this->configurationManager = configurationManager;
                if (this->configurationManager == nullptr) {
                    logger->wtf("ConfigPushMenuItem", "configurationManager is null");
                }
            }

            std::string ConfigPushMenuItem::getLabel() {
                return "Config Push (PicoToPi)";
            }

            void ConfigPushMenuItem::onItemClicked() {
                configurationStatusWriter->scheduleEmit(
                        configurationManager->getConfigurationCopy().toMessage());
            }
} // MainScreen