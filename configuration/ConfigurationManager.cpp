//
// Created by stefan on 11/20/22.
//

#include "ConfigurationManager.h"

namespace pico {
    namespace config {

        ConfigurationManager::ConfigurationManager(std::shared_ptr<FlashConfigurationStore> flashConfigurationStore,
                                                   std::shared_ptr<IBusConfigMessageStore> iBusConfigMessageStore,
                                                   std::shared_ptr<MemoryConfigurationStore> memoryConfigurationStore,
                                                   std::shared_ptr<logger::BaseLogger> logger,
                                                   std::shared_ptr<defaults::DefaultConfigurationProvider> defaultConfigurationProvider) {
            this->flashConfigurationStore = flashConfigurationStore;
            this->iBusConfigMessageStore = iBusConfigMessageStore;
            this->memoryConfigurationStore = memoryConfigurationStore;
            this->logger = logger;
            this->defaultConfigurationProvider = defaultConfigurationProvider;
        }

        std::unique_ptr<Configuration> ConfigurationManager::getMutableConfiguration() {
            if (memoryConfigurationStore->canReadConfiguration()) {
                logger->d("ConfigurationManager", "Using memory configuration store");
                Configuration memoryConfiguration = memoryConfigurationStore->getConfiguration();
                return std::unique_ptr<Configuration>(&memoryConfiguration);
            }

            if (flashConfigurationStore->canReadConfiguration()) {
                logger->d("ConfigurationManager", "Using flash configuration store");
                Configuration flashConfiguration = flashConfigurationStore->getConfiguration();
                return std::unique_ptr<Configuration>(&flashConfiguration);
            }

            logger->d("ConfigurationManager", "Getting new default configuration and saving it.");
            Configuration newDefault = defaultConfigurationProvider->getDefaultConfiguration();
            memoryConfigurationStore->saveConfiguration(newDefault);
//            flashConfigurationStore->saveConfiguration(newDefault);
            iBusConfigMessageStore->saveConfiguration(newDefault);

            return std::make_unique<Configuration>(newDefault);
        }

        void ConfigurationManager::saveConfiguration(std::unique_ptr<Configuration> configuration) {
            logger->d("ConfigurationManager", "Saving new configuration");

            if (!configuration->isSyncRequired) {
                logger->w("ConfigurationManager", "Asking to save a configuration that doesn't need a sync.");
                return;
            }

            logger->i("ConfigurationManager", "Saving configuration");

            Configuration configuration1 = *configuration;

            memoryConfigurationStore->saveConfiguration(configuration1);
//            flashConfigurationStore->saveConfiguration(configuration1);
            iBusConfigMessageStore->saveConfiguration(configuration1);
        }

        Configuration ConfigurationManager::getConfigurationCopy() {
            return Configuration(messages::ConfigMessage());
        }
    } // pico
} // config