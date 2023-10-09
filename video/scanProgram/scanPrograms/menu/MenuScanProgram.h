//
// Created by stefan on 9/1/23.
//

#ifndef PICOTEMPLATE_MENUSCANPROGRAM_H
#define PICOTEMPLATE_MENUSCANPROGRAM_H

#include "../BaseScanProgram.h"
#include "../graphicsLib/graphicsLib.h"
#include "../../../screenManager/Screen.h"
#include "../../../screenManager/ScreenItem.h"
#include "../../../screenManager/ScreenManager.h"

namespace video::scanProgram::scanPrograms::menu {

    class MenuScanProgram : public BaseScanProgram{

    private:
        std::shared_ptr<pico::logger::BaseLogger> logger;
        std::shared_ptr<video::scanProgram::graphicsLib> graphicsLib;

        std::shared_ptr<ScreenManager::ScreenManager> screenManager;

        void drawScreen(std::shared_ptr<ScreenManager::Screen> screen);

        void drawScreenBackground();

        void drawScreenMenuItems(std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>>);
        uint16_t drawScreenMenuItem(uint16_t tl, std::shared_ptr<video::ScreenManager::ScreenItem> item);

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
