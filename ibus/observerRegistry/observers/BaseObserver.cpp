//
// Created by stefan on 12/2/22.
//

#include <sstream>
#include "BaseObserver.h"

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

            messages::PiToPicoMessage BaseObserver::decodePiToPicoMessage(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    pico::ibus::data::IbusPacket ibusPacket
                    ) {
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


                //TODO This is some serious bullshit. This is an exercise for me someday to learn
                //TODO how to take a vector of ints, and convert it into a plain old c_str(), to jam
                //TODO into NanoPB_CPP's weird input stream type.
                auto* inputString = new std::string(0, '\0');
                for (const uint8_t item: *ibusPacket.getData()) {
                    inputString->append(reinterpret_cast<const char *>(item));
                }

                auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(inputString->c_str()));

                messages::PiToPicoMessage decoded;
                if(!NanoPb::decode<messages::PiToPicoMessageConverter>(inputStream, decoded)) {
                    logger->w(getTag(), "Failed to decode PiToPicoMessage");
                }

                return decoded;
            }
        } // pico
    } // ibus
} // observers