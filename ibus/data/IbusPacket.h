//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_IBUSPACKET_H
#define PICOTEMPLATE_IBUSPACKET_H

#include <memory>
#include <vector>
#include <cstddef>
#include "IbusDeviceEnum.h"

namespace pico {
    namespace ibus {
        namespace data {

            class IbusPacket : std::enable_shared_from_this<IbusPacket>{

            private:
                //The complete raw packet that the other fields are made of.
                std::vector<uint8_t> completeRawPacket;

                IbusDeviceEnum sourceDevice = static_cast<IbusDeviceEnum>(NULL);
                IbusDeviceEnum destinationDevice = static_cast<IbusDeviceEnum>(NULL);
                std::vector<uint8_t> data;
                uint8_t crc;

            public:
                IbusPacket(std::vector<uint8_t> completeRawPacket);

                IbusDeviceEnum getSourceDevice();
                IbusDeviceEnum getDestinationDevice();
                std::shared_ptr<std::vector<uint8_t>> getData();
                uint8_t getCrc();
                bool isPacketValid();

                std::string toString();
            };

        } // pico
    } // ibus
} // data

#endif //PICOTEMPLATE_IBUSPACKET_H
