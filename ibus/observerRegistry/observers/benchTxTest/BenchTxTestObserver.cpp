#include "BenchTxTestObserver.h"
#include <ibus/data/IBusDevices.h>

namespace pico::ibus::observers {

    BenchTxTestObserver::BenchTxTestObserver(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<dma::IDmaManager> dmaManager) {
        this->logger = logger;
        this->dmaManager = dmaManager;
    }

    void BenchTxTestObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        if (iBusPacket->getPacketSource() != data::FROM_CAR
            || (uint8_t) iBusPacket->getSourceDevice() != REQUEST_SRC
            || (uint8_t) iBusPacket->getDestinationDevice() != PICO_VALUE
            || iBusPacket->getDataLength() < 2) {
            return;
        }
        std::vector<uint8_t> raw = iBusPacket->getRawPacket();
        if (raw[data::IbusPacket::DATA_START] != MAGIC) {
            return;
        }
        uint8_t count = std::min<uint8_t>(raw[data::IbusPacket::DATA_START + 1], MAX_PACKETS_PER_REQUEST);
        requests++;
        logger->d(getTag(), fmt::format("Bench TX test request {}: queueing {} packets", requests, count));
        for (uint8_t seq = 0; seq < count; seq++) {
            dmaManager->cpu0scheduleOutgoingMessage(data::IbusPacket(
                    data::IbusDeviceEnum::PICO,
                    static_cast<data::IbusDeviceEnum>(TEST_DST),
                    {MAGIC, requests, seq, 0xA5, 0x5A, 0x00, 0xFF}));
        }
    }

} // observers
