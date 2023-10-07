//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_TEXTCOMMAND_H
#define PICOTEMPLATE_TEXTCOMMAND_H

#include <string>
#include <cstdint>
#include "PxCoord.h"
#include "BaseCommand.h"
namespace video::scanVideo::graphics::command {

    class TextCommand : public BaseCommand{


    public:
        enum Size { SMALL, MEDIUM, LARGE };

        TextCommand(
                std::string text,
                PxCoord topLeftPx,
                uint32_t colour,
                Size size
                );

        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;

        ~TextCommand() override = default;

    private:
        std::string text;
        PxCoord topLeftPx;
        uint32_t colour;
        Size size;

        uint8_t height_small = 10;
        uint8_t height_medium = 15;
        uint8_t height_large = 19;
    };

} // command

#endif //PICOTEMPLATE_TEXTCOMMAND_H
