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

        bool operator==(const PxCoord &rhs) const;

        bool operator!=(const PxCoord &rhs) const;

        uint16_t getX();
        uint16_t getY();

        bool operator<(const PxCoord &rhs) const {
            //sort by y, then x, because our other
            //data structures store map<scanline, rleRun>
            if (y < rhs.y)
                return true;
            if (rhs.y < y)
                return false;
            return x < rhs.x;
        }

        bool operator>(const PxCoord &rhs) const {
            return rhs < *this;
        }

        bool operator<=(const PxCoord &rhs) const {
            return !(rhs < *this);
        }

        bool operator>=(const PxCoord &rhs) const {
            return !(*this < rhs);
        }
    };

} // command

#endif //PICOTEMPLATE_PXCOORD_H
