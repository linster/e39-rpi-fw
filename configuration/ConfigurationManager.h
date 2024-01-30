//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_CONFIGURATIONMANAGER_H
#define PICOTEMPLATE_CONFIGURATIONMANAGER_H

#include <memory>
#include <logging/BaseLogger.h>
#include <configuration/store/flash/FlashConfigurationStore.h>
#include <configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.h>
#include <configuration/store/memory/MemoryConfigurationStore.h>
#include <configuration/default/DefaultConfigurationProvider.h>
namespace pico::config {

        class ConfigurationManager {

            //a singleton class that returns mutable configuration objects

        private:
            std::shared_ptr<FlashConfigurationStore> flashConfigurationStore;
            std::shared_ptr<IBusConfigMessageStore> iBusConfigMessageStore;
            std::shared_ptr<MemoryConfigurationStore> memoryConfigurationStore;

            std::shared_ptr<logger::BaseLogger> logger;

            std::shared_ptr<defaults::DefaultConfigurationProvider> defaultConfigurationProvider;
        public:
            ConfigurationManager(
                    std::shared_ptr<FlashConfigurationStore> flashConfigurationStore,
                    std::shared_ptr<IBusConfigMessageStore> iBusConfigMessageStore,
                    std::shared_ptr<MemoryConfigurationStore> memoryConfigurationStore,
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<defaults::DefaultConfigurationProvider> defaultConfigurationProvider
            );

            Configuration* getMutableConfiguration();
            void saveConfiguration(std::unique_ptr<Configuration> configuration);

            Configuration getConfigurationCopy();

        };

    } // config

#endif //PICOTEMPLATE_CONFIGURATIONMANAGER_H
