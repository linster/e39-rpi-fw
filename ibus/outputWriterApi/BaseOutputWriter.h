//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_BASEOUTPUTWRITER_H
#define PICOTEMPLATE_BASEOUTPUTWRITER_H

#include "../data/IbusPacket.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class BaseOutputWriter {

                protected:
                    //Puts the packet on a queue somewhere for writing.
                    //or, just blocks till it's done writing?
                    void schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket);
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_BASEOUTPUTWRITER_H
