//
// Created by stefan on 4/5/23.
//

#include "HeartbeatResponseWriter.h"

namespace pico::ibus::output::writer {
                HeartbeatResponseWriter::HeartbeatResponseWriter(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<dma::IDmaManager> dmaManager) {
                    this->logger = logger;
                    this->dmaManager = dmaManager;
                    if (this->dmaManager == nullptr) {
                        logger->wtf("HeartbeatResponseWriter", "dmaManager is null");
                    }
                }

                std::shared_ptr<dma::IDmaManager> HeartbeatResponseWriter::getDmaManager() {
                    return dmaManager;
                }

                void HeartbeatResponseWriter::sendHeartbeatResponse() {
                    logger->d(getTag(), "Sending heartbeat response");
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::HeartbeatResponse
                    });
                }
            } // writer