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

                //Payload is raw[DATA_START .. size - 1): skip src, len, dest; drop the trailing CRC.
                //Build the string from the byte range. This used to go through
                //std::string(inputString.c_str()), which stops at the first 0x00 byte and so cut
                //off any protobuf containing one (e.g. an empty sub-message is encoded "12 00").
                std::vector<uint8_t> raw = ibusPacket.getRawPacket(); //one copy, not one per byte
                if (raw.size() < data::IbusPacket::DATA_START + 1) {
                    logger->w(getTag(), "Packet too short to hold a PiToPicoMessage");
                    return messages::PiToPicoMessage{}; //messageType = EmptyMessage
                }

                auto payload = std::make_unique<std::string>(
                        raw.begin() + data::IbusPacket::DATA_START,
                        raw.end() - 1);

                auto inputStream = NanoPb::StringInputStream(std::move(payload));

                messages::PiToPicoMessage decoded;
                if(!NanoPb::decode<messages::PiToPicoMessageConverter>(inputStream, decoded)) {
                    logger->w(getTag(), "Failed to decode PiToPicoMessage");
                    //Never hand observers a half-decoded message: they act on messageType.
                    return messages::PiToPicoMessage{}; //messageType = EmptyMessage
                }

                return decoded;
            }
        } // observers