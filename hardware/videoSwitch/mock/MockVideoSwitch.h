//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_MOCKVIDEOSWITCH_H
#define PICOTEMPLATE_MOCKVIDEOSWITCH_H

#include <memory>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <logging/BaseLogger.h>
#include <libs/fmt/include/fmt/format.h>

namespace pico::hardware::videoSwitch::mock {

                class MockVideoSwitch : public pico::hardware::videoSwitch::VideoSwitch {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    VideoSource previousVideoSource;
                public:
                    MockVideoSwitch(
                            std::shared_ptr<logger::BaseLogger> baseLogger
                    );
                    void switchTo(pico::hardware::videoSwitch::VideoSource source) override;
                    VideoSource getPreviousVideoSource() override;
                };

            } // mock

#endif //PICOTEMPLATE_MOCKVIDEOSWITCH_H
