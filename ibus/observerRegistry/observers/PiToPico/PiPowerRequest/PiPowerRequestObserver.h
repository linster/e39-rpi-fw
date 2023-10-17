//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_PIPOWERREQUESTOBSERVER_H
#define PICOTEMPLATE_PIPOWERREQUESTOBSERVER_H


#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <logging/BaseLogger.h>
#include <ibus/data/IBusDevices.h>
#include <proto_cpp/PiToPico.h>
#include <hardware/pi4powerswitch/IPi4PowerSwitchManager.h>

namespace pico {
    namespace ibus {
        namespace observers {

            class PiPowerRequestObserver : public BaseObserver {

            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> powerSwitchManager;

                void onNewPiToPicoPacket(messages::PiToPicoMessage message);
            public:
                PiPowerRequestObserver(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> powerSwitchManager
                );
            protected:
                std::string getTag() override { return "PiPowerRequestObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_PIPOWERREQUESTOBSERVER_H
