//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
#define PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H

#include <vector>
#include <configuration/store/IConfigurationStore.h>
#include "flash_utils.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/multicore.h"

namespace pico::config {

        class FlashConfigurationStore : IConfigurationStore {

        private:

            std::shared_ptr<logger::BaseLogger> logger;

            std::pair<bool, std::vector<uint8_t>> encodeConfiguration(Configuration configuration);
            std::pair<bool, Configuration> decodeConfiguration();

            //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#mutex

            uint8_t* getPointerToConfigurationStorageInFlash();
            uint16_t getLengthOfConfigurationStorageInFlash();

            void saveConfigurationBytes(std::vector<uint8_t> config);

            /** This is set here so that we never return a dangling pointer to configuration.
             *
             */
            Configuration decodedConfiguration;

        public:

            explicit FlashConfigurationStore(
                    std::shared_ptr<logger::BaseLogger> logger
            );

            void saveConfiguration(Configuration configuration) override;
            Configuration* getConfiguration() override;
            bool canReadConfiguration() override;
        };

    } // config

#endif //PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
