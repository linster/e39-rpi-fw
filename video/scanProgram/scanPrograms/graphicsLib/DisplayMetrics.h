//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_DISPLAYMETRICS_H
#define PICOTEMPLATE_DISPLAYMETRICS_H

#include <cstdint>

namespace video::scanProgram {

        class DisplayMetrics {

        private:
            uint16_t displayHeightPx;
            uint16_t displayWidthPx;

        public:

            DisplayMetrics(
                   uint32_t displayHeightPx,
                   uint16_t displayWidthPx
            );

            uint16_t getDisplayHeightPx();
            uint16_t getDisplayWidthPx();
        };

    } // scanProgram

#endif //PICOTEMPLATE_DISPLAYMETRICS_H
