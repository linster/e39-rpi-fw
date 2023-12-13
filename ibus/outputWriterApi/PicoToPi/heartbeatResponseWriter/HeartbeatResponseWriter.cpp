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
                }

                std::shared_ptr<dma::IDmaManager> HeartbeatResponseWriter::getDmaManager() {
                    return dmaManager;
                }

                void HeartbeatResponseWriter::sendHeartbeatResponse() {
                    logger->d(getTag(), "Sending heartbeat response");
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage (
                            messages::PicoToPiMessage::MessageType::HeartbeatResponse
                    ));
                }
            } // writer