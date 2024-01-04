//
// Created by stefan on 9/1/23.
//

#include "MenuScanProgram.h"

namespace video::scanProgram::scanPrograms::menu {


    MenuScanProgram::MenuScanProgram(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::graphicsLib> bootsplashGraphicsLib,
            std::shared_ptr<video::scanProgram::graphicsLib> menuGraphicsLib,
            std::shared_ptr<ScreenManager::ScreenManager> screenManager) {
        this->logger = logger;

        this->bootsplashGraphicsLib = bootsplashGraphicsLib;
        this->menuGraphicsLib = menuGraphicsLib;

        this->bootsplashGraphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );

        this->menuGraphicsLib->setDisplayMetrics(
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
        );

        this->screenManager = screenManager;
        init(this->logger);
    }

    std::string MenuScanProgram::getTag() {
        return "MenuScanProgram";
    }

    void MenuScanProgram::onScanProgramStart() {
        logger->d(getTag(), "onScanProgramStart()");

        scanvideo_timing_enable(true);

        registerOnFocusChangeCallback();

        drawScreenBackground();

        logger->d(getTag(), "onScanProgramStart() drew screen background");

        drawScreen(screenManager->getCurrentScreen());
    }

    void MenuScanProgram::onScanProgramStop() {
        logger->d(getTag(), "onScanProgramStop()");

        menuGraphicsLib->clearFrame(); //Free up some memory

        unRegisterOnFocusChangeCallback();
    }

    void MenuScanProgram::render(scanvideo_scanline_buffer_t *scanline_buffer) {

        uint16_t menuTopScanline = 58;
        uint16_t menuBottomScanline = getDisplayHeightPx() - 40 - 2;

        uint16_t line_num = scanvideo_scanline_number(scanline_buffer->scanline_id);

        if (line_num < menuTopScanline || line_num > menuBottomScanline) {
            bootsplashGraphicsLib->render_commandProcessed(scanline_buffer);
        } else {
            menuGraphicsLib->render_commandProcessed(scanline_buffer);
        }
    }

    void MenuScanProgram::drawScreenBackground() {
        if (bootsplashGraphicsLib->hasGraphicsToRender()) {
            //The bootsplash already has the background, let's not waste cycles recomputing it.
            return;
        }

        logger->w(getTag(), "drawScreenBackground couldn't reuse the bootsplash background, redrawing.");

        bootsplash::BootsplashScanProgram::drawLinsterOs(
                bootsplashGraphicsLib,
                DisplayMetrics(getDisplayHeightPx(), getDisplayWidthPx())
                );
    }

    void MenuScanProgram::drawScreen(std::shared_ptr<ScreenManager::Screen> screen) {
        blankMenuItemArea();

        menuGraphicsLib->setImmediateMode(false);

        menuGraphicsLib->drawFilledRectangle(
            scanVideo::graphics::command::PxCoord(30, 58),
            scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 58, 58 + 8 + 2),
            menuGraphicsLib->getPalette()[14]
            );
        menuGraphicsLib->drawText(
                screen->getTitle(),
                scanVideo::graphics::command::PxCoord(90, 60),
                menuGraphicsLib->getPalette()[4],
                1
                );

        drawScreenMenuItems(screen->getScreenItems());

        menuGraphicsLib->computeFrame();
        menuGraphicsLib->setImmediateMode(true);
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
            menuGraphicsLib->drawEmptyRectangle(
                    scanVideo::graphics::command::PxCoord(menuItem_x, tl),
                    scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 58, tl + 8 + 6),
                    menuGraphicsLib->getPalette()[15],
                    2
            );
        }

        menuGraphicsLib->drawText(
                item->getLabel(),
                scanVideo::graphics::command::PxCoord(menuItem_x + 4, tl + 4),
                menuGraphicsLib->getPalette()[15],
                1
        );

        return 8 + 8;
    }

    void MenuScanProgram::blankMenuItemArea() {
        //menuGraphicsLib->clearFrame();
        menuGraphicsLib->drawFilledRectangle(
                scanVideo::graphics::command::PxCoord(30,58),
                scanVideo::graphics::command::PxCoord(getDisplayWidthPx() - 58, getDisplayHeightPx() - 40 - 2),
                graphicsLib::LINOS_BACKGROUND
                );
    }

    void MenuScanProgram::refreshUi() {
        logger->d(getTag(), "RefreshUI");
        scanvideo_wait_for_vblank();
        menuGraphicsLib->clearFrame();
        drawScreenBackground();
        drawScreen(screenManager->getCurrentScreen());
    }

    void MenuScanProgram::registerOnFocusChangeCallback() {
        std::function<void()> listener = [this]() {
            refreshUi();
        };
        screenManager->registerOnFocusChangeListener(listener);
    }

    void MenuScanProgram::unRegisterOnFocusChangeCallback() {
        screenManager->unregisterOnFocusChangeListener();
    }

} // menu