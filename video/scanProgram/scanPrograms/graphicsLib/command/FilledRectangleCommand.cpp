//
// Created by stefan on 10/7/23.
//

#include "FilledRectangleCommand.h"

namespace video::scanVideo::graphics::command {
    FilledRectangleCommand::FilledRectangleCommand(
            PxCoord topLeftPx,
            PxCoord bottomRightPx,
            uint32_t colour
            ) {

        this->topLeftPx = topLeftPx;
        this->bottomRightPx = bottomRightPx;
        this->colour = colour;

    }

    std::pair<uint16_t, uint16_t> FilledRectangleCommand::getAffectedScanlines() {
        return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), bottomRightPx.getY());
    }

    std::map<uint16_t, std::vector<RleRun>> FilledRectangleCommand::getRleRunsForShape() {
        std::map<uint16_t, std::vector<RleRun>> returnMap = std::map<uint16_t, std::vector<RleRun>>();

        uint16_t len = bottomRightPx.getX() - topLeftPx.getX();

        for (int i = topLeftPx.getY(); i <= bottomRightPx.getY(); i++) {
            std::vector<RleRun> runForLine = std::vector<RleRun>();
            runForLine.push_back(RleRun(
                    topLeftPx.getX(),
                    len,
                    colour
                    ));
            returnMap[i] = runForLine;
        }
        return returnMap;
    }
} // command