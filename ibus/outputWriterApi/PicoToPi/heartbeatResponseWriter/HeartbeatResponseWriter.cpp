//
// Created by stefan on 4/5/23.
//

#include "HeartbeatResponseWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {
                HeartbeatResponseWriter::HeartbeatResponseWriter(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<dma::DmaManager> dmaManager) {
                    this->logger = logger;
                    this->dmaManager = dmaManager;
                }

                std::shared_ptr<dma::DmaManager> HeartbeatResponseWriter::getDmaManager() {
                    return dmaManager;
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