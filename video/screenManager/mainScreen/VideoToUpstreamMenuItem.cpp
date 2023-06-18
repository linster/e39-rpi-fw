//
// Created by stefan on 6/18/23.
//

#include "VideoToUpstreamMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToUpstreamMenuItem::VideoToUpstreamMenuItem(
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch) {
                this->videoSwitch = videoSwitch;
            }

            void VideoToUpstreamMenuItem::onItemClicked() {
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::UPSTREAM);
            }

            std::string VideoToUpstreamMenuItem::getLabel() {
                return "Video to Upstream";
            }
} // MainScreen