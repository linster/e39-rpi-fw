//
// Created by stefan on 12/2/22.
//

#include <sstream>
#include "BaseObserver.h"

namespace pico::ibus::observers {

            void BaseObserver::dispatchPacket(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {

                if (log_packetDispatchTrace) {
                    logger->d(this->getTag(), fmt::format("Dispatching Packet"));
                }

                this->onNewPacket(iBusPacket);

                if (log_packetDispatchTrace) {
                    logger->d(this->getTag(), fmt::format("Dispatched Packet"));
                }
            }

            messages::PiToPicoMessage BaseObserver::decodePiToPicoMessage(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    pico::ibus::data::IbusPacket ibusPacket
                    ) {

                auto rawPacketSize = ibusPacket.getRawPacket().size();

                std::stringstream inputStringStream;

                std::string inputString = std::string(rawPacketSize - 4, '\0');

                for (int i = 3; i < rawPacketSize - 1; i++) {
                    inputStringStream << (char)ibusPacket.getRawPacket()[i];
                }

                inputString = inputStringStream.str();

                auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(inputString.c_str()));

                messages::PiToPicoMessage decoded;
                if(!NanoPb::decode<messages::PiToPicoMessageConverter>(inputStream, decoded)) {
                    logger->w(getTag(), "Failed to decode PiToPicoMessage");
                }

                return decoded;
            }
        } // observers