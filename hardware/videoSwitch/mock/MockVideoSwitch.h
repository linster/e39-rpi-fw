//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_MOCKVIDEOSWITCH_H
#define PICOTEMPLATE_MOCKVIDEOSWITCH_H

#include <memory>
#include "../VideoSwitch.h"
#include "../../../logging/BaseLogger.h"

namespace pico {
    namespace hardware {
        namespace videoSwitch {
            namespace mock {

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

            } // pico
        } // hardware
    } // videoSwitch
} // mock

#endif //PICOTEMPLATE_MOCKVIDEOSWITCH_H
