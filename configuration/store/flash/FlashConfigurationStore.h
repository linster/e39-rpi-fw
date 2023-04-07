//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
#define PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H

#include "../IConfigurationStore.h"
namespace pico {
    namespace config {

        class FlashConfigurationStore : IConfigurationStore {

        public:
            void saveConfiguration(Configuration configuration) override;
            Configuration getConfiguration() override;
            bool canReadConfiguration() override;
        };

    } // pico
} // config

#endif //PICOTEMPLATE_FLASHCONFIGURATIONSTORE_H
