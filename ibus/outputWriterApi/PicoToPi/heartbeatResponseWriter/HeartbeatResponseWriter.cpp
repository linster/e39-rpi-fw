//
// Created by stefan on 4/5/23.
//

#include "HeartbeatResponseWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {
                HeartbeatResponseWriter::HeartbeatResponseWriter(std::shared_ptr<logger::BaseLogger> looger) {
                    this->logger = looger;
                }

                void HeartbeatResponseWriter::sendHeartbeatResponse() {
                    logger->d(getTag(), "Sending heartbeat response");
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::HeartbeatResponse
                    });
                }
            } // pico
        } // ibus
    } // output
} // writer