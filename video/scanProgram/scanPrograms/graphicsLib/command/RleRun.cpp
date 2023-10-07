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

    RleRun RleRun::appendSameColor(RleRun other) {
        return RleRun(
                this->startX,
                this->len + other.len,
                this->colour);
    }

    uint16_t RleRun::getLen() { return len; }
    void RleRun::setLen(uint16_t newLen) { this->len = newLen; }

    uint16_t RleRun::getStartX() {return startX;}
    uint32_t RleRun::getColour() { return colour; }

    void RleRun::setColour(uint32_t colour) {
        this->colour = colour;
    }
} // command