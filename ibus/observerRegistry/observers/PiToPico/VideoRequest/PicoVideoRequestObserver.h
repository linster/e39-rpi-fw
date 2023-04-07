//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_PICOVIDEOREQUESTOBSERVER_H
#define PICOTEMPLATE_PICOVIDEOREQUESTOBSERVER_H

#include "../../BaseObserver.h"
#include "../../../logging/BaseLogger.h"
#include "../../../../data/IBusDevices.h"
#include "../../../../proto_cpp/PiToPico.h"
#include "../../hardware/videoSwitch/VideoSwitch.h"

namespace pico {
    namespace ibus {
        namespace observers {

            class PicoVideoRequestObserver : public BaseObserver {
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
                void onNewPiToPicoPacket(messages::PiToPicoMessage message);
            public:
                PicoVideoRequestObserver(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch
                        );
            protected:
                std::string getTag() override { return "PicoVideoRequestObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_PICOVIDEOREQUESTOBSERVER_H
