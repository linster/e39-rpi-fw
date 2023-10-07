//
// Created by stefan on 10/7/23.
//

#include "PxCoord.h"

namespace video::scanVideo::graphics::command {

    PxCoord::PxCoord(uint16_t x, uint16_t y) {
        this->x = x;
        this->y = y;
    }

    uint16_t PxCoord::getX() {
        return x;
    }

    uint16_t PxCoord::getY() {
        return y;
    }
} // command