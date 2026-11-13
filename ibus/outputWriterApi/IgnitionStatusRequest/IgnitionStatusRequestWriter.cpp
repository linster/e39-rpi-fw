//
// Created by stefan on 8/22/24.
//

#include "IgnitionStatusRequestWriter.h"

namespace pico::ibus::output::writer::ignition {

    IgnitionStatusRequestWriter::IgnitionStatusRequestWriter(std::shared_ptr<logger::BaseLogger> logger,
                                                             std::shared_ptr<dma::IDmaManager> dmaManager) {
        this->logger = logger;
        this->dmaManager = dmaManager;
    }

    std::shared_ptr<dma::IDmaManager> IgnitionStatusRequestWriter::getDmaManager() {
        return dmaManager;
    }

    void IgnitionStatusRequestWriter::sendIgnitionStatusRequest() {
        logger->d(getTag(), "sendIgnitionStatusRequest()");
        auto data = std::vector<uint8_t>(1);
        data.push_back(0x10);

        auto iBusPacket = data::IbusPacket(
                data::IbusDeviceEnum::PICO,
                data::IbusDeviceEnum::IKE,
                data
        );

        //https://github.com/piersholt/wilhelm-docs/blob/master/ike/10.md




        this->schedulePacketForWrite(iBusPacket);
    }
} // ignition