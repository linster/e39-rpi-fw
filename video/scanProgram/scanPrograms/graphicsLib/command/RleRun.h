//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_RLERUN_H
#define PICOTEMPLATE_RLERUN_H

#include <cstdint>
#include <vector>

namespace video::scanVideo::graphics::command {

    class RleRun {

    private:

        uint16_t startX = 0;
        uint16_t len = 0;
        uint32_t colour = 0;
    public:

        RleRun(
                uint16_t startX,
                uint16_t len,
                uint32_t colour
                );

        uint16_t getStartX();

        uint16_t getLen();
        void setLen(uint16_t newLen);

        uint32_t getColour();
        void setColour(uint32_t colour);

        RleRun appendSameColor(RleRun other);

    };

} // command

#endif //PICOTEMPLATE_RLERUN_H
