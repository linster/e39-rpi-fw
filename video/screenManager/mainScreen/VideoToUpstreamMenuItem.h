//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H
#define PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H

#include <memory>
#include <video/screenManager/ScreenItem.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include "video/scanProgram/ScanProgramSwapper.h"

namespace video::ScreenManager::MainScreen {

            class VideoToUpstreamMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
            public:
                VideoToUpstreamMenuItem(
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                        );

                std::string getLabel() override;
                void onItemClicked() override;

            };

        } // MainScreen

#endif //PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H
