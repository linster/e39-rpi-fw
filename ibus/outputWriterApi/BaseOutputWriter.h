//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_BASEOUTPUTWRITER_H
#define PICOTEMPLATE_BASEOUTPUTWRITER_H

#include <ibus/data/IbusPacket.h>
#include <proto_cpp/PicoToPi.h>
#include <ibus/dma/IDmaManager.h>

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class BaseOutputWriter {

                protected:

                    virtual std::string getTag() = 0;

                    virtual std::shared_ptr<dma::IDmaManager> getDmaManager() = 0;

                    //Puts the packet on a queue somewhere for writing.
                    //or, just blocks till it's done writing?
                    void schedulePacketForWrite(pico::ibus::data::IbusPacket ibusPacket);

                    void schedulePicoToPiMessageForWrite(messages::PicoToPiMessage message);
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_BASEOUTPUTWRITER_H
