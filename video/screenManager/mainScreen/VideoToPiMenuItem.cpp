//
// Created by stefan on 6/18/23.
//

#include "VideoToPiMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToPiMenuItem::VideoToPiMenuItem(
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                    ) {
                this->videoSwitch = videoSwitch;
                this->scanProgramSwapper = scanProgramSwapper;
            }

            std::string VideoToPiMenuItem::getLabel() {
                return "Video to Pi";
            }

            void VideoToPiMenuItem::onItemClicked() {
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::PI);
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
            }
} // MainScreen