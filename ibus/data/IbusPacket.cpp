//
// Created by stefan on 12/2/22.
//

#include "IbusPacket.h"
#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace data {

            IbusPacket::IbusPacket(char *packetStart) {
                //Super dangerous constructor that takes a pointer to a byte
                //then increments it to get the length, dangerously reads that
                //many bytes and makes a vector of ints.

                //TODO
            }

            IbusPacket::IbusPacket(std::vector<uint8_t> raw) {
                sourceDevice = static_cast<IbusDeviceEnum>(raw[0]);
                packetLength = raw[1]; //TODO this should never be bigger than raw.length()
                destinationDevice = static_cast<IbusDeviceEnum>(raw[2]);

                completeRawPacket = raw;

                if (packetLength <= 2) {
                    data = std::vector<uint8_t>();
                } else {
                    // subtract 2 because length includes checksum and dest address
                    auto begin = raw.begin() + 3;
                    auto end = raw.end() - 1;
                    data.insert(data.begin(), begin, end);
                }

                givenCrc = raw.back();

                //Now calculate the actual CRC.
                actualCrc = 0;
                for (auto iterator = raw.end(); iterator != (raw.end() - 1); ++iterator) {
                    //Skip the CRC byte at the end
                    actualCrc = actualCrc ^ *iterator;
                }
            }

            IbusPacket::IbusPacket(IbusDeviceEnum src, IbusDeviceEnum dest, std::vector<uint8_t> data) {

                sourceDevice = src;
                destinationDevice = dest;
                this->data = std::vector<uint8_t>(data);

                this->packetLength = data.size() + 4;

                //Now, build up the completeRawPacket

                //Calculate the actual CRC
                //Set the givenCRC to actual

                //pop the CRC on the end of the completeRawPacket.

                //TODO

            }

            IbusPacket::~IbusPacket() {
                data.clear();
                data.shrink_to_fit();
                completeRawPacket.clear();
                completeRawPacket.shrink_to_fit();
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

            uint8_t IbusPacket::getGivenCrc() {
                return givenCrc;
            }

            uint8_t IbusPacket::getActualCrc() {
                return actualCrc;
            }

            bool IbusPacket::isPacketValid() {
                return givenCrc == actualCrc;
            }

            std::string IbusPacket::toString() {
                return fmt::format("IbusPacket( "
                                   "SourceDevice: {} "
                                   "DestDevice: {}, "
                                   "Data: {})",
                                   sourceDevice,
                                   destinationDevice,
                                   fmt::join(data, ","));
            }

            std::vector<uint8_t> IbusPacket::getRawPacket() {
                return completeRawPacket;
            }


        } // pico
    } // ibus
} // data