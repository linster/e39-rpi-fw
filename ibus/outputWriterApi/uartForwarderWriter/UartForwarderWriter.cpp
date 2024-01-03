//
// Created by stefan on 1/2/24.
//

#include "UartForwarderWriter.h"

namespace pico::ibus::output::writer {
    UartForwarderWriter::UartForwarderWriter(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<dma::IDmaManager> dmaManager
            ) {
        this->logger = logger;
        this->dmaManager = dmaManager;
    }

    std::shared_ptr<dma::IDmaManager> UartForwarderWriter::getDmaManager() {
        return dmaManager;
    }

    void UartForwarderWriter::forwardPacketToCar(data::IbusPacket packet) {
        getDmaManager()->cpu0scheduleOutgoingMessage(packet);
    }

    void UartForwarderWriter::forwardPacketToPi(data::IbusPacket packet) {
        getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(packet);
    }

} // writer