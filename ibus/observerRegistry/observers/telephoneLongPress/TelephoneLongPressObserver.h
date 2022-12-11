//
// Created by stefan on 12/10/22.
//

#ifndef PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H
#define PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H

#include "../BaseObserver.h"
#include "../../../../hardware/videoSwitch/VideoSwitch.h"
#include "../../../../video/scanProgram/ScanProgramSwapper.h"

namespace pico {
    namespace ibus {
        namespace observers {

            class TelephoneLongPressObserver : public BaseObserver{

            public:
                TelephoneLongPressObserver(
                        std::shared_ptr<logger::BaseLogger> baseLogger,
                        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper
                );
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;

                void onTelephoneLongPressed();
            protected:
                std::string getTag() override { return "TelephoneLongPressObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H
