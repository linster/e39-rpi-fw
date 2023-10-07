//
// Created by stefan on 10/7/23.
//

#include "DisplayMetrics.h"

namespace video::scanProgram {

    DisplayMetrics::DisplayMetrics(
            uint32_t displayHeightPx,
            uint16_t displayWidthPx
    ) {
        this->displayHeightPx = displayHeightPx;
        this->displayWidthPx = displayWidthPx;
    }

    uint16_t DisplayMetrics::getDisplayHeightPx() {
        return displayHeightPx;
    }

    uint16_t DisplayMetrics::getDisplayWidthPx() {
        return displayWidthPx;
    }


} // scanProgram