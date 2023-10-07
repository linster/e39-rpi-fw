//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_BASECOMMAND_H
#define PICOTEMPLATE_BASECOMMAND_H

#include <cstdint>
#include <utility>
#include <map>
#include "RleRun.h"
namespace video::scanVideo::graphics::command {

    class BaseCommand {

    public:

        //Affected scanlines
        //Iterator pattern: once we're on a scanline, get an RLE token for a colour run
        //after that, loop over the runs we get. For a particular scanline.

        //Each command contributes an ordered RLE run for a scanline.


        virtual std::pair<uint16_t, uint16_t> getAffectedScanlines() = 0;

        virtual std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() = 0;

        virtual ~BaseCommand() = default;; //TODO each subclass should implement the virtual dtor?

        //TODO we should also made operator= virtual?

    };

} // command

#endif //PICOTEMPLATE_BASECOMMAND_H
