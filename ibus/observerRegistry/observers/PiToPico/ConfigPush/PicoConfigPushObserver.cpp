//
// Created by stefan on 2/22/24.
//

#include "PicoConfigPushObserver.h"

namespace pico::ibus::observers {
    PicoConfigPushObserver::PicoConfigPushObserver(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter,
            std::shared_ptr<config::ConfigurationManager> configurationManager) {
        this->logger = logger;
        this->configurationStatusWriter = configurationStatusWriter;
        this->configurationManager = configurationManager;
    }

    void PicoConfigPushObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        if (iBusPacket->getSourceDevice() == PI_VALUE && iBusPacket->getDestinationDevice() == PICO_VALUE) {
            messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, *iBusPacket);
            onNewPiToPicoPacket(decoded);
        }
    }

    void PicoConfigPushObserver::onNewPiToPicoPacket(messages::PiToPicoMessage message) {
        if (message.messageType == messages::PiToPicoMessage::MessageType::ConfigPush) {

            configurationManager->saveConfiguration(
                    std::make_unique<config::Configuration>(config::Configuration(message.newConfig))
                            );

            configurationStatusWriter->scheduleEmit(
                    configurationManager->getConfigurationCopy().toMessage()
                    );
        }
    }


} // observers