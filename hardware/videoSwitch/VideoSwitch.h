//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_VIDEOSWITCH_H
#define PICOTEMPLATE_VIDEOSWITCH_H

#include "VideoSource.h"

namespace pico {
    namespace hardware {
        namespace videoSwitch {

            class VideoSwitch {
            public:
                virtual void switchTo(VideoSource source) = 0;
                virtual VideoSource getPreviousVideoSource() = 0;
            };

        } // pico
    } // hardware
} // videoSwitch

#endif //PICOTEMPLATE_VIDEOSWITCH_H