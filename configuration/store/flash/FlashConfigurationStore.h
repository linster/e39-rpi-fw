//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
#define PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H

#include <vector>
#include <configuration/store/IConfigurationStore.h>
namespace pico::config {

        class FlashConfigurationStore : IConfigurationStore {

        private:

            std::shared_ptr<logger::BaseLogger> logger;

            std::pair<bool, std::vector<uint8_t>> encodeConfiguration(Configuration configuration);
            std::pair<bool, Configuration> decodeConfiguration(uint8_t * ptr, uint16_t len);

            //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#mutex

            uint8_t * getPointerToConfigurationStorageInFlash();
            uint16_t getLengthOfConfigurationStorageInFlash();

            void saveConfigurationBytes(uint8_t * ptr, std::vector<uint8_t>);

        public:

            FlashConfigurationStore(
                    std::shared_ptr<logger::BaseLogger> logger
            );

            void saveConfiguration(Configuration configuration) override;
            Configuration getConfiguration() override;
            bool canReadConfiguration() override;
        };

    } // config

#endif //PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
