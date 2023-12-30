//
// Created by stefan on 6/18/23.
//

#include "VideoToUpstreamMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToUpstreamMenuItem::VideoToUpstreamMenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper) {
                this->logger = logger;
                this->videoSwitch = videoSwitch;
                if (this->videoSwitch == nullptr) {
                    logger->wtf("VideoToRvcMenuItem", "videoSwitch is null");
                }
                this->scanProgramSwapper = scanProgramSwapper;
                if (this->scanProgramSwapper == nullptr) {
                    logger->wtf("VideoToRvcMenuItem", "scanProgramSwapper is null");
                }
            }

            void VideoToUpstreamMenuItem::onItemClicked() {
                logger->d("VideoToUpstreamMenuItem", "Setting videoSwitch to upstream");
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::UPSTREAM);
                logger->d("VideoToUpstreamMenuItem", "Have set videoSwitch to upstream");

                logger->d("VideoToUpstreamMenuItem", "Setting scan program to noop");
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
                logger->d("VideoToUpstreamMenuItem", "Have set scan program to noop");
            }

            std::string VideoToUpstreamMenuItem::getLabel() {
                return "Video to Upstream";
            }
} // MainScreen