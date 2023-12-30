//
// Created by stefan on 4/5/23.
//

#include "SoftPowerRequestWriter.h"

namespace pico::ibus::output::writer {

    SoftPowerRequestWriter::SoftPowerRequestWriter(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<dma::IDmaManager> dmaManager) {
        this->logger = logger;
        this->dmaManager = dmaManager;
        if (this->dmaManager == nullptr) {
            logger->wtf("SoftPowerRequestWriter", "dmaManager is null");
        }
    }

    std::shared_ptr<dma::IDmaManager> SoftPowerRequestWriter::getDmaManager() {
        return this->dmaManager;
    }

    void SoftPowerRequestWriter::requestRpiRestart() {
        logger->d(getTag(), "Requesting rpi soft restart");
        schedulePicoToPiMessageForWrite(messages::PicoToPiMessage{
                .messageType = messages::PicoToPiMessage::MessageType::PiSoftPowerRestartPi
        });
    }

    void SoftPowerRequestWriter::requestRpiRestartX() {
        logger->d(getTag(), "Requesting rpi restart X server.");
        schedulePicoToPiMessageForWrite(messages::PicoToPiMessage{
                .messageType = messages::PicoToPiMessage::MessageType::PiSoftPowerRestartX
        });
    }


} // writer