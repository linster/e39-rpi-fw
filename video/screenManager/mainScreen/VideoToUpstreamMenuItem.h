//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H
#define PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H

#include <memory>
#include "../ScreenItem.h"
#include "../../../hardware/videoSwitch/VideoSwitch.h"

namespace video {
    namespace ScreenManager {
        namespace MainScreen {

            class VideoToUpstreamMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
            public:
                VideoToUpstreamMenuItem(
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch
                        );

                std::string getLabel() override;
                void onItemClicked() override;

            };

        } // video
    } // ScreenManager
} // MainScreen

#endif //PICOTEMPLATE_VIDEOTOUPSTREAMMENUITEM_H
