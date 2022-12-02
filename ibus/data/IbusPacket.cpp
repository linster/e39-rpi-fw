//
// Created by stefan on 12/2/22.
//

#include "IbusPacket.h"

namespace pico {
    namespace ibus {
        namespace data {


            IbusPacket::IbusPacket(std::vector<uint8_t> completeRawPacket) {

            }

            IbusDeviceEnum IbusPacket::getSourceDevice() {
                return sourceDevice;
            }

            IbusDeviceEnum IbusPacket::getDestinationDevice() {
                return destinationDevice;
            }

            std::shared_ptr<std::vector<uint8_t>> IbusPacket::getData() {
                return std::make_shared<std::vector<uint8_t>>(data);
            }

            uint8_t IbusPacket::getCrc() {
                return crc;
            }

            bool IbusPacket::isPacketValid() {
                return false;
            }

            std::string IbusPacket::toString() {
                return "NOPE";
            }


        } // pico
    } // ibus
} // data