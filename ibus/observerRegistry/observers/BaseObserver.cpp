//
// Created by stefan on 12/2/22.
//

#include "BaseObserver.h"
#include "../../../libs/fmt/include/fmt/format.h"

namespace pico {
    namespace ibus {
        namespace observers {

            void BaseObserver::dispatchPacket(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    pico::ibus::data::IbusPacket iBusPacket) {

                logger->d(this->getTag(), fmt::format("Dispatching Packet"));
                this->onNewPacket(iBusPacket);
                logger->d(this->getTag(), fmt::format("Dispatched Packet"));
            }

            messages::PiToPicoMessage BaseObserver::decodePiToPicoMessage(pico::ibus::data::IbusPacket ibusPacket) {
                auto foo = iBusPacket.getData();

                //std::shared_ptr<std::vector<uint8_t>> IbusPacket::getData() {
                //TODO convert this to a string somehow?
                //TODO or a string buffer?

                ///* Create an input stream for reading from a memory buffer.
                //*
                //* msglen should be the actual length of the message, not the full size of
                //* allocated buffer.
                //*
                //* Alternatively, you can use a custom stream that reads directly from e.g.
                //* a file or a network socket.
                //*/
                //pb_istream_t pb_istream_from_buffer(const pb_byte_t *buf, size_t msglen);


                auto inputStream = NanoPb::StringInputStream();

            }
        } // pico
    } // ibus
} // observers