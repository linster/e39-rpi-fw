//
// Created by stefan on 10/7/23.
//

#include "RleRun.h"

namespace video::scanVideo::graphics::command {
    RleRun::RleRun(uint16_t startX, uint16_t len, uint32_t colour) {
        this->startX = startX;
        this->len = len;
        this->colour = colour;
    }
} // command