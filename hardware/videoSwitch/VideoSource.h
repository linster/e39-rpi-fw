//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_VIDEOSOURCE_H
#define PICOTEMPLATE_VIDEOSOURCE_H

#include <string>

namespace pico {
    namespace hardware {
        namespace videoSwitch {

            enum VideoSource { RVC = 0x0, UPSTREAM = 0x1, PICO = 0x2, PI = 0x3};
        } // pico
    } // hardware
} // videoSwitch

#endif //PICOTEMPLATE_VIDEOSOURCE_H
