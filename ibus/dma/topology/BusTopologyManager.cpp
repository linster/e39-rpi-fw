//
// Created by stefan on 1/1/24.
//

#include "BusTopologyManager.h"

namespace pico::ibus::topology {
    BusTopology BusTopologyManager::getBusToplogy() {
        //TODO read some args set from CMake.
        return BusTopology::SLED_NO_PI;
    }
} // topology