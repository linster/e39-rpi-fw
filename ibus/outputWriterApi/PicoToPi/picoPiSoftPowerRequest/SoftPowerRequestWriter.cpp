//
// Created by stefan on 4/5/23.
//

#include "SoftPowerRequestWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                SoftPowerRequestWriter::SoftPowerRequestWriter(std::shared_ptr<logger::BaseLogger> logger) {
                    this->logger = logger;
                }

                void SoftPowerRequestWriter::requestRpiRestart() {
                    logger->d(getTag(), "Requesting rpi soft restart");
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::PiSoftPowerRestartPi
                    });
                }

                void SoftPowerRequestWriter::requestRpiRestartX() {
                    logger->d(getTag(), "Requesting rpi restart X server.");
                    schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                        .messageType = messages::PicoToPiMessage::MessageType::PiSoftPowerRestartX
                    });
                }

            } // pico
        } // ibus
    } // output
} // writer