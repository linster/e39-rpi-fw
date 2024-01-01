//
// Created by stefan on 1/1/24.
//

#include "BusTopologyManager.h"

namespace pico::ibus::topology {
    BusTopology BusTopologyManager::getBusToplogy() {
        return BusTopology::SLED_NO_PI;
    }
} // topology