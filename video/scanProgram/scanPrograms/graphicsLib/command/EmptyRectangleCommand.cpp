//
// Created by stefan on 10/7/23.
//

#include "EmptyRectangleCommand.h"

namespace video::scanVideo::graphics::command {
    EmptyRectangleCommand::EmptyRectangleCommand(
            PxCoord topLeftPx,
            PxCoord bottomRightPx,
            uint32_t colour,
            uint8_t lineWidth) {

        this->topLeftPx = topLeftPx;
        this->bottomRightPx = bottomRightPx;
        this->colour = colour;
        this->lineWidth = lineWidth;
    }

    std::pair<uint16_t, uint16_t> EmptyRectangleCommand::getAffectedScanlines() {
        return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), bottomRightPx.getY());
    }

    std::map<uint16_t, std::vector<RleRun>> EmptyRectangleCommand::getRleRunsForShape() {
        std::map<uint16_t, std::vector<RleRun>> returnMap = std::map<uint16_t, std::vector<RleRun>>();

        uint16_t len = bottomRightPx.getX() - topLeftPx.getX();

        for (int i = topLeftPx.getY(); i <= bottomRightPx.getY(); i++) {
            std::vector<RleRun> runForLine = std::vector<RleRun>();

            if (i == topLeftPx.getY()
                    || i < topLeftPx.getY() + lineWidth
                    || i == bottomRightPx.getY()
                    || i > bottomRightPx.getY() - lineWidth
                ) {
                runForLine.push_back(RleRun(
                        topLeftPx.getX(),
                        len,
                        colour
                ));
            } else {
                runForLine.push_back(RleRun(
                        topLeftPx.getX(),
                        lineWidth,
                        colour
                ));
                runForLine.push_back(RleRun(
                        bottomRightPx.getX() - (lineWidth),
                        lineWidth,
                        colour
                ));
            }
            returnMap[i] = runForLine;
        }
        return returnMap;
    }
} // command