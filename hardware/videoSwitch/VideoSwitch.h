//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_VIDEOSWITCH_H
#define PICOTEMPLATE_VIDEOSWITCH_H

#include "VideoSource.h"

namespace pico::hardware::videoSwitch {

            class VideoSwitch {
            public:
                virtual void switchTo(VideoSource source) = 0;
                virtual VideoSource getPreviousVideoSource() = 0;
            };

        } // videoSwitch

#endif //PICOTEMPLATE_VIDEOSWITCH_H
