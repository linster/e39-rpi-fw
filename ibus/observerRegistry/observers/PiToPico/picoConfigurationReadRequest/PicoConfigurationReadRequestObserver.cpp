//
// Created by stefan on 12/11/22.
//

#include "PicoConfigurationReadRequestObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            PicoConfigurationReadRequestObserver::PicoConfigurationReadRequestObserver(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                    std::shared_ptr<config::ConfigurationManager> configurationManager) {

                this->logger = logger;
                this->configurationStatusWriter = configurationStatusWriter;
                this->configurationManager = configurationManager;
            }

            void PicoConfigurationReadRequestObserver::onNewPacket(pico::ibus::data::IbusPacket iBusPacket) {
                if (iBusPacket.getSourceDevice() == PI_VALUE && iBusPacket.getDestinationDevice() == PICO_VALUE) {
                    messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, iBusPacket);
                    onNewPiToPicoPacket(decoded);
                }
            }

            void PicoConfigurationReadRequestObserver::onNewPiToPicoPacket(messages::PiToPicoMessage message) {
                if (message.messageType == messages::PiToPicoMessage::MessageType::ConfigStatusRequest) {
                    logger->d(getTag(), "Got a config request from Pi.");
                    configurationStatusWriter->scheduleEmit(
                            configurationManager->getConfigurationCopy().toMessage()
                            );
                }
            }

        } // pico
    } // ibus
} // observers