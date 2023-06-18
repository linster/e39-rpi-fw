//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_VIDEOTORVCMENUITEM_H
#define PICOTEMPLATE_VIDEOTORVCMENUITEM_H

#include <memory>
#include "../ScreenItem.h"
#include "../../../hardware/videoSwitch/VideoSwitch.h"

namespace video {
    namespace ScreenManager {
        namespace MainScreen {

            class VideoToRvcMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
            public:
                VideoToRvcMenuItem(
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch
                        );

                void onItemClicked() override;
                std::string getLabel() override;
            };

        } // video
    } // ScreenManager
} // MainScreen

#endif //PICOTEMPLATE_VIDEOTORVCMENUITEM_H
