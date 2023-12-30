//
// Created by stefan on 6/18/23.
//

#include "VideoToPiMenuItem.h"

namespace video::ScreenManager::MainScreen {

            VideoToPiMenuItem::VideoToPiMenuItem(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                    ) {
                this->logger = logger;
                this->videoSwitch = videoSwitch;
                if (this->videoSwitch == nullptr) {
                    logger->wtf("VideoToPiMenuItem", "videoSwitch is null");
                }
                this->scanProgramSwapper = scanProgramSwapper;
                if (this->scanProgramSwapper == nullptr) {
                    logger->wtf("VideoToPiMenuItem", "scanProgramSwapper is null");
                }
            }

            std::string VideoToPiMenuItem::getLabel() {
                return "Video to Pi";
            }

            void VideoToPiMenuItem::onItemClicked() {
                videoSwitch->switchTo(pico::hardware::videoSwitch::VideoSource::PI);
                scanProgramSwapper->swapTo(ScanProgram::NOOP);
            }
} // MainScreen