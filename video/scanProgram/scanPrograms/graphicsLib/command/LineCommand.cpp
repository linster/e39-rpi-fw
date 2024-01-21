//
// Created by stefan on 10/7/23.
//

#include <valarray>
#include <set>
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

        for (int i = topLeftPx.getY(); i <= bottomRightPx.getY(); i++) {
            std::vector<RleRun> runForLine = std::vector<RleRun>();
            runForLine.push_back(RleRun(
                    topLeftPx.getX(),
                    lineWidth,
                    colour
            ));
            returnMap[i] = runForLine;
        }
        return returnMap;
    }

    std::map<uint16_t, std::vector<RleRun>> LineCommand::getRleRunsForAngledLine() {

        //TODO bresenham's stair-step algorithm for lines here.
        //http://members.chello.at/~easyfilter/bresenham.html


        std::map<uint16_t, std::vector<RleRun>> returnMap = std::map<uint16_t, std::vector<RleRun>>();

        std::set<PxCoord> pixelsToSet = std::set<PxCoord>();

        int16_t x0 = topLeftPx.getX();
        int16_t x1 = bottomRightPx.getX();
        int16_t y0 = topLeftPx.getY();
        int16_t y1 = bottomRightPx.getY();

        int16_t dx =  std::abs(x1-x0);
        int16_t sx = x0<x1 ? 1 : -1;

        int16_t dy = -std::abs(y1-y0);
        int16_t sy = y0<y1 ? 1 : -1;

        int16_t err = dx+dy, e2; /* error value e_xy */

        for(;;){

            pixelsToSet.emplace(x0, y0);

            if (x0==x1 && y0==y1) break;
            e2 = 2*err;
            if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
            if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
        }

        //TODO sort by Y
        //TODO use the vector as a queue, pop off each element, make an RLE run for it,
        //TODO and insert into map.

        for (PxCoord pixel: pixelsToSet) {
            if (returnMap.count(pixel.getY()) == 0) {
                returnMap[pixel.getY()] = std::vector<RleRun>();
            }

            returnMap[pixel.getY()].emplace_back(pixel.getX(), lineWidth, colour);
        }

        return returnMap;
    }
} // command