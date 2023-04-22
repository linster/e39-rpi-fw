//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
#define PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H

#include <vector>
#include "../IConfigurationStore.h"
namespace pico {
    namespace config {

        class FlashConfigurationStore : IConfigurationStore {

        private:
            std::vector<uint8_t> encodeConfiguration(Configuration configuration);
            Configuration decodeConfiguration(void* ptr);

            //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#mutex


        public:
            void saveConfiguration(Configuration configuration) override;
            Configuration getConfiguration() override;
            bool canReadConfiguration() override;
        };

    } // pico
} // config

#endif //PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
