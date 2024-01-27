//
// Created by stefan on 11/20/22.
//

#include "IConfigurationStore.h"

namespace pico::config {
        bool IConfigurationStore::canReadConfiguration() {
            return false;
        }

} // config