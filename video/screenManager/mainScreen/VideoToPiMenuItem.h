//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTOPIMENUITEM_H
#define PICOTEMPLATE_VIDEOTOPIMENUITEM_H

#include <memory>
#include <video/screenManager/ScreenItem.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include "video/scanProgram/ScanProgramSwapper.h"

namespace video::ScreenManager::MainScreen {

            class VideoToPiMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::logger::BaseLogger> logger;
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
            public:
                VideoToPiMenuItem(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                        );

                void onItemClicked() override;
                std::string getLabel() override;
            };

        } // MainScreen

#endif //PICOTEMPLATE_VIDEOTOPIMENUITEM_H
