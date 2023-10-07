//
// Created by stefan on 10/7/23.
//

#include "LineCommand.h"

namespace video::scanVideo::graphics::command {
    LineCommand::LineCommand(
            PxCoord topLeftPx,
            PxCoord bottomRightPx,
            uint32_t colour,
            uint8_t lineWidth
            ) {
        this->topLeftPx = topLeftPx;
        this->bottomRightPx = bottomRightPx;
        this->colour = colour;
        this->lineWidth = lineWidth;
    }

    std::pair<uint16_t, uint16_t> LineCommand::getAffectedScanlines() {
        if (topLeftPx.getY() == bottomRightPx.getY()) {
            return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), topLeftPx.getY() + lineWidth - 1);
        }
        return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), bottomRightPx.getY());
    }

    std::map<uint16_t, std::vector<RleRun>> LineCommand::getRleRunsForShape() {
        if (topLeftPx.getY() == bottomRightPx.getY()) {
            return getRleRunsForHorizontalLine();
        }
        if (topLeftPx.getX() == bottomRightPx.getX()) {
            return getRleRunsForVerticalLine();
        }
        return getRleRunsForAngledLine();
    }

    std::map<uint16_t, std::vector<RleRun>> LineCommand::getRleRunsForHorizontalLine() {
        std::map<uint16_t, std::vector<RleRun>> returnMap = std::map<uint16_t, std::vector<RleRun>>();

        uint16_t len = bottomRightPx.getX() - topLeftPx.getX();

        for (int i = topLeftPx.getY(); i <= topLeftPx.getY() + lineWidth - 1; i++) {
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

    std::map<uint16_t, std::vector<RleRun>> LineCommand::getRleRunsForVerticalLine() {
        std::map<uint16_t, std::vector<RleRun>> returnMap = std::map<uint16_t, std::vector<RleRun>>();

        uint16_t len = topLeftPx.getX() + lineWidth;

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

    std::map<uint16_t, std::vector<RleRun>> LineCommand::getRleRunsForAngledLine() {
        //TODO bresenham's stair-step algorithm for lines here.
        return std::map<uint16_t, std::vector<RleRun>>();
    }
} // command