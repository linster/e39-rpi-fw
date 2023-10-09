//
// Created by stefan on 9/1/23.
//

#include "MenuScanProgram.h"

namespace video::scanProgram::scanPrograms::menu {


    MenuScanProgram::MenuScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib,
            std::shared_ptr<ScreenManager::ScreenManager> screenManager) {
        this->logger = logger;
        this->graphicsLib = graphicsLib;
        this->graphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );
        this->screenManager = screenManager;
        init(logger);
    }

    std::string MenuScanProgram::getTag() {
        return "MenuScanProgram";
    }

    void MenuScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");

        drawScreenBackground();

        drawScreen(screenManager->getCurrentScreen());
    }

    void MenuScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");
    }

    void MenuScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {

        //TODO use the frame number to re-ask the screen manager the current state
        //TODO every so many frames (not every frame, because re-drawing the screen
        //TODO is expensive.

        graphicsLib->render_commandProcessed(scanline_buffer);
    }

    void MenuScanProgram::drawScreenBackground() {
        uint8_t topColourHeight = 40;
        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(1,1),
                scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 2,getDisplayHeightPx() - topColourHeight),
                PICO_COLOR_FROM_RGB5(0, 0, 8)
        );

        uint8_t linOs_x = 5;
        uint8_t linOs_y = 20;
        uint8_t linOs_height = 2;
        std::string linOs_text = std::string("Linster OS");
        graphicsLib->drawText(
                linOs_text,
                scanVideo::graphics::command::PxCoord(linOs_x + 3, linOs_y + 2),
                graphicsLib->getPalette()[0],
                linOs_height
        );
        graphicsLib->drawText(
                linOs_text,
                scanVideo::graphics::command::PxCoord(linOs_x, linOs_y),
                graphicsLib->getPalette()[15],
                linOs_height
        );

        std::string automotive_text = std::string("Automotive");
        uint8_t automotive_x = linOs_x + (8 * linOs_height);
        uint8_t automotive_y = linOs_y + (8 * linOs_height) + 8;
        uint8_t automotive_height = 1;
        graphicsLib->drawText(
                "Automotive",
                scanVideo::graphics::command::PxCoord(automotive_x, automotive_y),
                graphicsLib->getPalette()[15],
                automotive_height
        );

        graphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(1,getDisplayHeightPx() - topColourHeight),
                scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 2,getDisplayHeightPx() - 2),
                graphicsLib->getPalette()[14]
        );

        uint32_t copyrightColour = PICO_COLOR_FROM_RGB5(0, 0, 8);
        graphicsLib->drawText(
                "e39-Rpi",
                scanVideo::graphics::command::PxCoord(10, getDisplayHeightPx() - topColourHeight + 8),
                copyrightColour,
                1
        );

        graphicsLib->drawText(
                "  Stefan Martynkiw 2019-2024",
                scanVideo::graphics::command::PxCoord(10, getDisplayHeightPx() - topColourHeight + 10 + 10),
                copyrightColour,
                1
        );

        graphicsLib->drawTextSpecialCharacter(
                graphicsLib::SPECIAL_CHARACTER_COPYRIGHT,
                scanVideo::graphics::command::PxCoord(20, getDisplayHeightPx() - topColourHeight + 10 + 10),
                copyrightColour,
                1
        );
    }

    void MenuScanProgram::drawScreen(std::shared_ptr<ScreenManager::Screen> screen) {

        graphicsLib->drawFilledRectangle(
            scanVideo::graphics::command::PxCoord(30, 58),
            scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 58, 58 + 8 + 2),
            graphicsLib->getPalette()[14]
            );
        graphicsLib->drawText(
                screen->getTitle(),
                scanVideo::graphics::command::PxCoord(90, 60),
                graphicsLib->getPalette()[4],
                1
                );

        drawScreenMenuItems(screen->getScreenItems());
    }

    void MenuScanProgram::drawScreenMenuItems(std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>> screenItems) {

        uint16_t menuItem_tl_y = 75;

        for (const auto item : screenItems) {
            menuItem_tl_y = menuItem_tl_y + drawScreenMenuItem(menuItem_tl_y, item);
        }
    }

    uint16_t MenuScanProgram::drawScreenMenuItem(uint16_t tl, std::shared_ptr<ScreenManager::ScreenItem> item) {

        uint16_t menuItem_x = 30;

        if (item->getIsFocused()) {
            graphicsLib->drawEmptyRectangle(
                    scanVideo::graphics::command::PxCoord(menuItem_x, tl),
                    scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 58, tl + 8 + 6),
                    graphicsLib->getPalette()[15],
                    2
            );
        }

        graphicsLib->drawText(
                item->getLabel(),
                scanVideo::graphics::command::PxCoord(menuItem_x + 4, tl + 4),
                graphicsLib->getPalette()[15],
                1
        );

        return 8 + 8;
    }

} // menu