//
// Created by stefan on 6/18/23.
//

#include "VideoToRvcMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToRvcMenuItem::VideoToRvcMenuItem(
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper) {
                this->videoSwitch = videoSwitch;
                this->scanProgramSwapper = scanProgramSwapper;
            }

            std::string VideoToRvcMenuItem::getLabel() {
                return "Video to RVC";
            }

            void VideoToRvcMenuItem::onItemClicked() {
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::RVC);
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
            }
} // MainScreen