//
// Created by stefan on 4/5/23.
//

#include "ConfigurationStatusWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                ConfigurationStatusWriter::ConfigurationStatusWriter(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<dma::DmaManager> dmaManager
                ) {
                    this->logger = logger;
                    this->dmaManager = dmaManager;
                }

                std::shared_ptr<dma::DmaManager> ConfigurationStatusWriter::getDmaManager() {
                    return dmaManager;
                }

                void ConfigurationStatusWriter::scheduleEmit(messages::ConfigMessage configMessage) {
                    this->schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::ConfigStatusResponse,
                        .existingConfig = configMessage
                    });
                }
            } // pico
        } // ibus
    } // output
} // writer