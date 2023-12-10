//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_HEATBEATREQUESTOBSERVER_H
#define PICOTEMPLATE_HEATBEATREQUESTOBSERVER_H

#include <logging/BaseLogger.h>
#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h>
#include <ibus/data/IBusDevices.h>
#include <proto_cpp/PiToPico.h>

namespace pico {
    namespace ibus {
        namespace observers {

            class HeatbeatRequestObserver : public BaseObserver {

            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<output::writer::HeartbeatResponseWriter> heartbeatResponseWriter;

                void onNewPiToPicoPacket(messages::PiToPicoMessage message);
            public:
                HeatbeatRequestObserver(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<output::writer::HeartbeatResponseWriter> heartbeatResponseWriter
                );
            protected:
                std::string getTag() override { return "HeatbeatRequestObserver"; };
                void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_HEATBEATREQUESTOBSERVER_H
