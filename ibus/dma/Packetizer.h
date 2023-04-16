//
// Created by stefan on 4/15/23.
//

#ifndef PICOTEMPLATE_PACKETIZER_H
#define PICOTEMPLATE_PACKETIZER_H

#include <vector>
#include <cstdint>

namespace pico {
    namespace ibus {
        namespace dma {

            class Packetizer {

            private:
                std::vector<uint8_t> packetBytes;
                bool calculateInProgressCrcValid();
            public:

                Packetizer();
                ~Packetizer();

                void addBytes(std::vector<uint8_t> bytes);
                void addByte(uint8_t byte);

                bool isPacketComplete();

                std::vector<uint8_t> getPacketBytes();

                //Don't actually clear everything out. From start to end of the buffer,
                //grab a complete packet, and remove it. Take what's left and shuffle it to the
                //beginning of the buffer. Then, that way we don't miss any in-flight packets,
                //and can keep building up sequential partial packets.
                void recycle();

                //Call whenever we want to clear all the state. Maybe we've had too many invalid packets in a row
                //noticed downstream, so we should clear this state out and give it a chance to rebuild a decent
                //flow again. (Call from a consumer of a B->P queue, and count up how many invalid ibus packets we
                //got out of this).
                void reset();
            };

        } // pico
    } // ibus
} // dma

#endif //PICOTEMPLATE_PACKETIZER_H
