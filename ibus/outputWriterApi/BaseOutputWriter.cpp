//
// Created by stefan on 12/2/22.
//

#include "BaseOutputWriter.h"
#include "../ibus/data/IBusDevices.h"
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

                    //TODO convert this into a vector of bytes.

                    getDmaManager()->cpu0scheduleOutgoingProbeOnlyMessage(
                            data::IbusPacket(
                                    PICO_VALUE,
                                    PI_VALUE,

                            ));

                }

                void BaseOutputWriter::schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket) {
                    getDmaManager()->cpu0scheduleOutgoingMessage(ibusPacket);
                }
            } // pico
        } // ibus
    } // output
} // writer