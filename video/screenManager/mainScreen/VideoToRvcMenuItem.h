//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTORVCMENUITEM_H
#define PICOTEMPLATE_VIDEOTORVCMENUITEM_H

#include <memory>
#include <video/screenManager/ScreenItem.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include "video/scanProgram/ScanProgramSwapper.h"

namespace video::ScreenManager::MainScreen {

            class VideoToRvcMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::logger::BaseLogger> logger;
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
            public:
                VideoToRvcMenuItem(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                        );

                void onItemClicked() override;
                std::string getLabel() override;
            };

        } // MainScreen

#endif //PICOTEMPLATE_VIDEOTORVCMENUITEM_H
