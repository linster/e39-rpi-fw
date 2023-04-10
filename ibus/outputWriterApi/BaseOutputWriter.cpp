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
                }

                void BaseOutputWriter::schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket) {
                    getDmaManager()->cpu0scheduleOutgoingMessage(ibusPacket);
                }
            } // pico
        } // ibus
    } // output
} // writer