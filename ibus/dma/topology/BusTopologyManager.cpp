//
// Created by stefan on 1/1/24.
//

#include "BusTopologyManager.h"

namespace pico::ibus::topology {
    BusTopology BusTopologyManager::getBusToplogy() {
        #ifdef CMAKE_BUS_TOPOLOGY

            #if (CMAKE_BUS_TOPOLOGY == CMAKE_BUS_TOPOLOGY_CAR_WITH_PI)
                return BusTopology::CAR_WITH_PI;
            #endif

            #if (CMAKE_BUS_TOPOLOGY == CMAKE_BUS_TOPOLOGY_SLED_NO_PI)
                return BusTopology::SLED_NO_PI;
            #endif

            #if (CMAKE_BUS_TOPOLOGY == CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI)
                return BusTopology::SLED_LAPTOP_HMI;
            #endif
        #else
            return BusTopology::SLED_NO_PI;
        #endif
    }
} // topology