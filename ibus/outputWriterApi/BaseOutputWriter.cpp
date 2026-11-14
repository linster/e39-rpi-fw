//
// Created by stefan on 12/2/22.
//

#include "BaseOutputWriter.h"
namespace pico::ibus::output::writer {

                void BaseOutputWriter::schedulePicoToPiMessageForWrite(messages::PicoToPiMessage message) {

                    //IBus packet = src, len, dest, payload, crc and must fit a 255-byte DMA queue
                    //item, so the payload can be at most 251 bytes. (Was 255, which made packets of
                    //up to 259 bytes.) Larger messages now fail to encode instead.
                    NanoPb::StringOutputStream outputStream = NanoPb::StringOutputStream(255 - 4);
                    if (!NanoPb::encode<messages::PicoToPiMessageConverter>(outputStream, message)) {
                        //TODO WTF
                        //TODO can't log here, sooooo?
                        return;
                    }

                    std::unique_ptr<std::basic_string<char>> p = outputStream.release();
//                    p->c_str();
//                    p->length();

                    //TODO might have to trim null terminator when looping through the string?
//
                    auto bytes = std::vector<uint8_t>();
                    for (char c: *p) {
                        bytes.push_back(c);
                    }
//
//                    for (auto iterator = p->begin(); iterator != (p->end() - 1); ++iterator) {
//                        bytes.push_back(*iterator);
//                    }

                    data::IbusPacket outgoingPacket = data::IbusPacket(
                            data::IbusDeviceEnum::PICO,
                            data::IbusDeviceEnum::RPI,
                            bytes
                    );

                    getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(outgoingPacket);

                }

                void BaseOutputWriter::schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket) {
                    getDmaManager()->cpu0scheduleOutgoingMessage(ibusPacket);
                }
            } // writer