//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTOPIMENUITEM_H
#define PICOTEMPLATE_VIDEOTOPIMENUITEM_H

#include <memory>
#include "../ScreenItem.h"
#include "../../../hardware/videoSwitch/VideoSwitch.h"

namespace video::ScreenManager::MainScreen {

            class VideoToPiMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
            public:
                VideoToPiMenuItem(
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch
                        );

                void onItemClicked() override;
                std::string getLabel() override;
            };

        } // MainScreen

#endif //PICOTEMPLATE_VIDEOTOPIMENUITEM_H
