//
// Created by stefan on 12/2/22.
//

#include "BaseOutputWriter.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                void BaseOutputWriter::schedulePicoToPiMessageForWrite(messages::PicoToPiMessage message) {
                    //LOL NOPE
                    //TODO
                    //TODO there should be a way to write this to the Pi
                    //TODO but not the rest of the car, as this might be really
                    //TODO chatty.


                    NanoPb::StringOutputStream outputStream;
                    if (!NanoPb::encode<messages::PicoToPiMessageConverter>(outputStream, message)) {
                        //TODO WTF
                        return;
                    }

                    std::unique_ptr<std::basic_string<char>> p = outputStream.release();
//                    p->c_str();
//                    p->length();

                    auto bytes = std::vector<uint8_t>();
                    for (char c: *p) {
                        bytes.push_back(c);
                    }

                    getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(
                            data::IbusPacket(
                                    data::IbusDeviceEnum::PICO,
                                    data::IbusDeviceEnum::RPI,
                                    std::move(bytes)
                            ));

                }

                void BaseOutputWriter::schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket) {
                    getDmaManager()->cpu0scheduleOutgoingMessage(ibusPacket);
                }
            } // pico
        } // ibus
    } // output
} // writer