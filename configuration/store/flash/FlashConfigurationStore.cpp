//
// Created by stefan on 4/5/23.
//

#include "FlashConfigurationStore.h"

namespace pico {
    namespace config {

        bool FlashConfigurationStore::canReadConfiguration() {
            //TODO Try-catch reading a block of flash memory
            return false;
        }

        Configuration FlashConfigurationStore::getConfiguration() {
            //TODO read the block of memory
        }

        void FlashConfigurationStore::saveConfiguration(Configuration configuration) {
            //TODO save the block
        }

    } // pico
} // config