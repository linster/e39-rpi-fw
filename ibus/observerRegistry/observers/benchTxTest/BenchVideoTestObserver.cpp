#include "BenchVideoTestObserver.h"
#include <ibus/data/IBusDevices.h>
#include <video/scanProgram/ScanProgramManager.h>

namespace pico::ibus::observers {

    BenchVideoTestObserver::BenchVideoTestObserver(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper) {
        this->logger = logger;
        this->scanProgramSwapper = scanProgramSwapper;
    }

    void BenchVideoTestObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        if (iBusPacket->getPacketSource() != data::FROM_CAR
            || (uint8_t) iBusPacket->getSourceDevice() != REQUEST_SRC
            || (uint8_t) iBusPacket->getDestinationDevice() != PICO_VALUE
            || iBusPacket->getDataLength() < 2) {
            return;
        }
        std::vector<uint8_t> raw = iBusPacket->getRawPacket();
        uint8_t command = raw[data::IbusPacket::DATA_START];
        uint8_t arg = raw[data::IbusPacket::DATA_START + 1];
        if (command == SWAP && arg <= (uint8_t) ScanProgram::NOOP) {
            logger->d(getTag(), fmt::format("Bench: swap to scan program {}", arg));
            scanProgramSwapper->swapTo(static_cast<ScanProgram>(arg));
        } else if (command == STALL_CORE1) {
            logger->d(getTag(), fmt::format("Bench: stall core 1 for {} ms", arg));
            video::scanProgram::ScanProgramManager::benchCore1StallMs = arg;
        }
    }

} // observers
