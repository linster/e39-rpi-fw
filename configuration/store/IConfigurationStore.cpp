//
// Created by stefan on 11/20/22.
//

#include "IConfigurationStore.h"

namespace pico {
    namespace config {
        bool IConfigurationStore::canReadConfiguration() {
            return false;
        }

        void IConfigurationStore::saveConfiguration(Configuration configuration) {
        }

        Configuration IConfigurationStore::getConfiguration() {
            return Configuration(messages::ConfigMessage());
        }
    } // pico
} // config