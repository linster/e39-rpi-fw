//
// Created by stefan on 10/7/23.
//

#include "TextCommand.h"

namespace video::scanVideo::graphics::command {
    TextCommand::TextCommand(
            std::string text,
            PxCoord topLeftPx,
            uint32_t colour,
            TextCommand::Size size
            ) {

        this->text = text;
        this->topLeftPx = topLeftPx;
        this->colour = colour;
        this->size = size;
    }

    std::pair<uint16_t, uint16_t> TextCommand::getAffectedScanlines() {
        uint8_t height = 0;
        switch(size) {
            case SMALL: height = height_small; break;
            case MEDIUM: height = height_medium; break;
            case LARGE: height = height_large; break;
        }
        return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), topLeftPx.getY() + height);
    }
} // command