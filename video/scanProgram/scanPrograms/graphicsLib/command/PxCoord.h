//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_PXCOORD_H
#define PICOTEMPLATE_PXCOORD_H

#include <cstdint>
namespace video::scanVideo::graphics::command {

    class PxCoord {

    private:
        uint16_t x = 0;
        uint16_t y = 0;

    public:
        PxCoord() = default;
        PxCoord(
                uint16_t x,
                uint16_t y
                );

        uint16_t getX();
        uint16_t getY();
    };

} // command

#endif //PICOTEMPLATE_PXCOORD_H
