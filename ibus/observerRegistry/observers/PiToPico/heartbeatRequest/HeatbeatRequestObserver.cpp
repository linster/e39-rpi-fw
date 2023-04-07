//
// Created by stefan on 4/5/23.
//

#include "HeatbeatRequestObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            HeatbeatRequestObserver::HeatbeatRequestObserver(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<output::writer::HeartbeatResponseWriter> heartbeatResponseWriter) {
                this->logger = logger;
                this->heartbeatResponseWriter = heartbeatResponseWriter;
            }

            void HeatbeatRequestObserver::onNewPacket(pico::ibus::data::IbusPacket iBusPacket) {
                if (iBusPacket.getSourceDevice() == PI_VALUE && iBusPacket.getDestinationDevice() == PICO_VALUE) {
                    messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, iBusPacket);
                    onNewPiToPicoPacket(decoded);
                }
            }

            void HeatbeatRequestObserver::onNewPiToPicoPacket(messages::PiToPicoMessage message) {
                if (message.messageType == messages::PiToPicoMessage::MessageType::HeartbeatRequest) {
                    logger->d(getTag(), "Got a heartbeat request from Pi.");
                    heartbeatResponseWriter->sendHeartbeatResponse();
                }
            }

        } // pico
    } // ibus
} // observers