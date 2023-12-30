//
// Created by stefan on 6/18/23.
//

#include "VideoToRvcMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToRvcMenuItem::VideoToRvcMenuItem(
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

            std::string VideoToRvcMenuItem::getLabel() {
                return "Video to RVC";
            }

            void VideoToRvcMenuItem::onItemClicked() {
                logger->d("VideoToRvcMenuItem", "Setting video source to rvc");
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::RVC);
                logger->d("VideoToRvcMenuItem", "Have set video source to rvc");

                logger->d("VideoToRvcMenuItem", "Setting scan program to noop");
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
                logger->d("VideoToRvcMenuItem", "Have set scan program to noop");
            }
} // MainScreen