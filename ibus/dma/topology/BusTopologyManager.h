//
// Created by stefan on 1/1/24.
//

#ifndef PICOTEMPLATE_BUSTOPOLOGYMANAGER_H
#define PICOTEMPLATE_BUSTOPOLOGYMANAGER_H

#include "PicoBusTopology.h"

namespace pico::ibus::topology {

    class BusTopologyManager {

    public:
        BusTopology getBusToplogy();
    };

} // topology

#endif //PICOTEMPLATE_BUSTOPOLOGYMANAGER_H
