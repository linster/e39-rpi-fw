//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_FILLEDRECTANGLECOMMAND_H
#define PICOTEMPLATE_FILLEDRECTANGLECOMMAND_H

#include <cstdint>
#include "PxCoord.h"
#include "BaseCommand.h"
namespace video::scanVideo::graphics::command {

    class FilledRectangleCommand : public BaseCommand {

    private:
        PxCoord topLeftPx;
        PxCoord bottomRightPx;
        uint32_t colour;

    public:
        FilledRectangleCommand(
                PxCoord topLeftPx,
                PxCoord bottomRightPx,
                uint32_t colour
                );

        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;

    };

} // command

#endif //PICOTEMPLATE_FILLEDRECTANGLECOMMAND_H
