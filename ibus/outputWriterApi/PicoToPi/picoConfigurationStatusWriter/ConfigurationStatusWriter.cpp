//
// Created by stefan on 4/5/23.
//

#include "ConfigurationStatusWriter.h"

namespace pico::ibus::output::writer {

    ConfigurationStatusWriter::ConfigurationStatusWriter(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<dma::IDmaManager> dmaManager
    ) {
        this->logger = logger;
        this->dmaManager = dmaManager;
        if (this->dmaManager == nullptr) {
            logger->wtf("ConfigurationStatusWriter", "dmaManager is null");
        }
    }

    std::shared_ptr<dma::IDmaManager> ConfigurationStatusWriter::getDmaManager() {
        return dmaManager;
    }

    void ConfigurationStatusWriter::scheduleEmit(messages::ConfigMessage configMessage) {
        this->schedulePicoToPiMessageForWrite(messages::PicoToPiMessage{
                .messageType = messages::PicoToPiMessage::MessageType::ConfigStatusResponse,
                .existingConfig = configMessage
        });
    }
} // writer