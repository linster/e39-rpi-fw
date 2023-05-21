//
// Created by stefan on 11/21/22.
//

#ifndef PICOTEMPLATE_MAX4314VIDEOSWITCH_H
#define PICOTEMPLATE_MAX4314VIDEOSWITCH_H

#include <memory>
#include "../VideoSwitch.h"
#include "../../../logging/BaseLogger.h"

namespace pico {
    namespace hardware {
        namespace videoSwitch {
            namespace max4314 {

                class Max4314VideoSwitch : public pico::hardware::videoSwitch::VideoSwitch {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    VideoSource previousVideoSource;
                    int gpio_pin_lsb = 19; //GP19
                    int gpio_pin_msb = 20; //GP20
                public:
                    Max4314VideoSwitch(
                            std::shared_ptr<logger::BaseLogger> baseLogger
                    );
                    void switchTo(pico::hardware::videoSwitch::VideoSource source) override;
                    VideoSource getPreviousVideoSource() override;
                };

            } // pico
        } // hardware
    } // videoSwitch
} // max4314

#endif //PICOTEMPLATE_MAX4314VIDEOSWITCH_H
