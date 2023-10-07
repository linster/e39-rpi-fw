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


        //We can use a fold-right to keep splitting RLEs on a scanline
        //or reduce-right?
        /**
         * Split this RleRun with another one.
         *
         * Cases:
         *
         * Split non-overlap:
         *
         * AAA
         *     BBB
         * Returns:
         * AAA (base-case. if the vector returned has one item in it, no need to split the right?)
         * TODO am I setting myself up for recursion here?
         *
         * Split complete-overlap:
         *
         * AAA
         * B
         * Returns:
         * B, AA
         *
         * AAA
         *  B
         * Returns:
         * A, B, A
         *
         * AAA
         *  BB
         * Returns:
         * A, BB
         *
         * AAA
         * BBB
         * Returns:
         * BBB
         *
         * Split partial-overlap:
         *
         * AAA
         *   BB
         * Returns:
         * AA, BB (TODO why not AA, B, B ?)
         *
         * AAA
         *  BBB
         * Returns:
         * A, BBB
         *
         * @param other The other RleRun to split this one with
         * @return A pair. First item is the left-most RleRun. Second item is the second-most RleRun
         */
        std::vector<RleRun> splitWith(RleRun other);

    };

} // command

#endif //PICOTEMPLATE_RLERUN_H
