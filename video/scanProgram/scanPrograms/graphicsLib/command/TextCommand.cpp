//
// Created by stefan on 10/7/23.
//

#include "TextCommand.h"

namespace video::scanVideo::graphics::command {
    TextCommand::TextCommand(
            std::string text,
            PxCoord topLeftPx,
            uint32_t colour,
            uint8_t pixelSize
            ) {

        this->text = text;
        this->topLeftPx = topLeftPx;
        this->colour = colour;
        this->pixelSize = pixelSize;
    }

    std::pair<uint16_t, uint16_t> TextCommand::getAffectedScanlines() {
        return std::pair<uint16_t, uint16_t>(topLeftPx.getY(), topLeftPx.getY() + (8 * pixelSize));
    }

    std::map<uint16_t, std::vector<RleRun>> TextCommand::getRleRunsForShape() {

        std::map<uint16_t, std::vector<RleRun>> retRuns = std::map<uint16_t, std::vector<RleRun>>();

        int index = 1;
        for (auto c : text){
            std::map<uint16_t, std::vector<RleRun>> charRuns = getRleRunsForCharacter(index++, c);

            for (auto l : charRuns) {
                if (retRuns.count(l.first) == 0) {
                    retRuns[l.first] = std::vector<RleRun>();
                }
                for (auto r : l.second) {
                    retRuns[l.first].emplace_back(r);
                }
            }
        }
        return retRuns;
    }

    std::map<uint16_t, std::vector<RleRun>> TextCommand::getRleRunsForCharacter(uint8_t index, char c) {
        std::map<uint16_t, std::vector<RleRun>> retRuns = std::map<uint16_t, std::vector<RleRun>>();

        uint8_t * bitmap;


        int ord = (int)c;
        if (ord > 127 || ord < 0) {
            //The only special character we support is the copyright symbol lol
            //TODO someday actually figure out how atoi() handles non-ASCII characters
            bitmap = (uint8_t *) fonts::FontProvider::font8x8_ext_latin[9];
        }
        bitmap = (uint8_t *) fonts::FontProvider::font8x8_basic[ord];

        return getRleRunsForBitmap(index, bitmap);
    }

    std::map<uint16_t, std::vector<RleRun>> TextCommand::getRleRunsForSpecialCharacter(uint8_t *bitmap) {
        return getRleRunsForBitmap(0, bitmap);
    }

    std::map<uint16_t, std::vector<RleRun>> TextCommand::getRleRunsForBitmap(uint8_t index, uint8_t *bitmap) {
        std::map<uint16_t, std::vector<RleRun>> retRuns = std::map<uint16_t, std::vector<RleRun>>();
        bool set;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                set = bitmap[row] & (1 << col);

                if (set) {
                    if (pixelSize == 1) {
                        //Base case
                        uint16_t y = topLeftPx.getY() + row;
                        RleRun onePixelRun = RleRun((topLeftPx.getX() + (9 * index)) + col, 1, colour);
                        if (retRuns.count(y) == 0) {
                            retRuns[y] = std::vector<RleRun>();
                        }
                        retRuns[y].push_back(onePixelRun);
                    } else {
                        RleRun bigPixelRun = RleRun(topLeftPx.getX() + (9 * index * pixelSize) + (col * pixelSize), pixelSize, colour);
                        for (int i = 0; i < pixelSize; ++i) {
                            uint16_t y = topLeftPx.getY() + i + (row * pixelSize);
                            if (retRuns.count(y) == 0) {
                                retRuns[y] = std::vector<RleRun>();
                            }
                            retRuns[y].push_back(bigPixelRun);
                        }
                    }
                }

            }
        }
        return retRuns;
    }


} // command