//
// Created by stefan on 10/7/23.
//

#ifndef PICOTEMPLATE_LINECOMMAND_H
#define PICOTEMPLATE_LINECOMMAND_H

#include <cstdint>
#include "PxCoord.h"
#include "BaseCommand.h"
namespace video::scanVideo::graphics::command {

    class LineCommand : public BaseCommand{

    private:
        PxCoord topLeftPx;
        PxCoord bottomRightPx;
        uint32_t colour;
        uint8_t lineWidth;

        std::map<uint16_t, std::vector<RleRun>> getRleRunsForHorizontalLine();
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForVerticalLine();

        //Bresenham line algorithms ftw
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForAngledLine();


    public:
        LineCommand(
                PxCoord topLeftPx,
                PxCoord bottomRightPx,
                uint32_t colour,
                uint8_t lineWidth
        );

        std::pair<uint16_t, uint16_t> getAffectedScanlines() override;
        std::map<uint16_t, std::vector<RleRun>> getRleRunsForShape() override;
    };

} // command

#endif //PICOTEMPLATE_LINECOMMAND_H
