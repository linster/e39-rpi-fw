//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_TEXTCOMMAND_H
#define PICOTEMPLATE_TEXTCOMMAND_H

#include <string>
#include <cstdint>
#include "PxCoord.h"
#include "BaseCommand.h"

#include <video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.h>

namespace video::scanVideo::graphics::command {

    class TextCommand : public BaseCommand{



    private:
        std::string text;
        PxCoord topLeftPx;
        uint32_t colour;
        uint8_t pixelSize;

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForCharacter(uint8_t index, char c);

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForBitmap(uint8_t index, uint8_t* bitmap);

    public:

        TextCommand(
                std::string text,
                PxCoord topLeftPx,
                uint32_t colour,
                uint8_t pixelSize
        );

        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;

        ~TextCommand() override = default;

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForSpecialCharacter(uint8_t* bitmap);

    };

} // command

#endif //PICOTEMPLATE_TEXTCOMMAND_H
