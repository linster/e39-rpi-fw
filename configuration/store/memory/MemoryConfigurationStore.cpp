//
// Created by stefan on 11/20/22.
//

#include "MemoryConfigurationStore.h"

namespace pico {
    namespace config {

        MemoryConfigurationStore::MemoryConfigurationStore() {}
        bool MemoryConfigurationStore::canReadConfiguration() {
            return hasConfiguration;
        }

        Configuration MemoryConfigurationStore::getConfiguration() {
            return *inMemoryConfiguration;
        }

        void MemoryConfigurationStore::saveConfiguration(Configuration configuration) {
            inMemoryConfiguration = &configuration;
            hasConfiguration = true;
        }
    } // pico
} // config