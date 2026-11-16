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

                    //What the schedule*ForWrite() helpers use. The default copies the shared_ptr,
                    //which is fine for writers that only run on core 0. A writer that can run on
                    //core 1 (the IBus logger) must override this so it never touches the
                    //(non-atomic) reference count from core 1.
                    virtual dma::IDmaManager* getDmaManagerForWrite() { return getDmaManager().get(); }

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
