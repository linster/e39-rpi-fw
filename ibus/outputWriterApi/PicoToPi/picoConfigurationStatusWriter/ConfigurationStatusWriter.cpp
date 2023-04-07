//
// Created by stefan on 4/5/23.
//

#include "ConfigurationStatusWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                ConfigurationStatusWriter::ConfigurationStatusWriter(
                        std::shared_ptr<logger::BaseLogger> logger
                ) {
                    this->logger = logger;
                }

                void ConfigurationStatusWriter::scheduleEmit(messages::ConfigMessage configMessage) {
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::ConfigStatusResponse,
                        .existingConfig = configMessage
                    });
                }
            } // pico
        } // ibus
    } // output
} // writer