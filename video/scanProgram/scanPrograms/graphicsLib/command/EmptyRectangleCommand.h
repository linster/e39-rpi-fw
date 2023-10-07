//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_EMPTYRECTANGLECOMMAND_H
#define PICOTEMPLATE_EMPTYRECTANGLECOMMAND_H

#include <cstdint>
#include "PxCoord.h"
#include "BaseCommand.h"
namespace video::scanVideo::graphics::command {

    class EmptyRectangleCommand : public BaseCommand {

    private:
        PxCoord topLeftPx;
        PxCoord bottomRightPx;
        uint32_t colour;

        //Inset from the topLeft -> bottomRight box.
        uint8_t lineWidth;

    public:

        EmptyRectangleCommand(
                PxCoord topLeftPx,
                PxCoord bottomRightPx,
                uint32_t colour,
                uint8_t lineWidth
        );

        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;

        ~EmptyRectangleCommand() override = default;

    };

} // command

#endif //PICOTEMPLATE_EMPTYRECTANGLECOMMAND_H
