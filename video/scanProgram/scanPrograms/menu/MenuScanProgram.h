//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_MENUSCANPROGRAM_H
#define PICOTEMPLATE_MENUSCANPROGRAM_H

#include <video/scanProgram/scanPrograms/BaseScanProgram.h>
#include <video/scanProgram/scanPrograms/graphicsLib/graphicsLib.h>
#include <video/screenManager/Screen.h>
#include <video/screenManager/ScreenItem.h>
#include <video/screenManager/ScreenManager.h>

namespace video::scanProgram::scanPrograms::menu {

    class MenuScanProgram : public BaseScanProgram{

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;

        std::shared_ptr<ScreenManager::ScreenManager> screenManager;

        uint8_t framesPerSecond = 24;
        uint16_t previousFrameNumber = 0;

        void drawScreen(std::shared_ptr<ScreenManager::Screen> screen);

        void drawScreenBackground();

        void drawScreenMenuItems(std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>>);
        uint16_t drawScreenMenuItem(uint16_t tl, std::shared_ptr<video::ScreenManager::ScreenItem> item);

        void blankMenuItemArea();
    public:
        MenuScanProgram(
                std::shared_ptr<pico::logger::BaseLogger> logger,
                std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib,
                std::shared_ptr<ScreenManager::ScreenManager> screenManager
        );

    protected:
        std::string getTag() override;
        void onScanProgramStart() override;
        void onScanProgramStop() override;
        void render(scanvideo_scanline_buffer_t *scanline_buffer) override;
    };

            } // menu

#endif //PICOTEMPLATE_MENUSCANPROGRAM_H
