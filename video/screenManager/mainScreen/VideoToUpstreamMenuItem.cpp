//
// Created by stefan on 6/18/23.
//

#include "VideoToUpstreamMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToUpstreamMenuItem::VideoToUpstreamMenuItem(
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper) {
                this->videoSwitch = videoSwitch;
                this->scanProgramSwapper = scanProgramSwapper;
            }

            void VideoToUpstreamMenuItem::onItemClicked() {
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::UPSTREAM);
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
            }

            std::string VideoToUpstreamMenuItem::getLabel() {
                return "Video to Upstream";
            }
} // MainScreen