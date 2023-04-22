//
// Created by stefan on 12/2/22.
//

#include "IbusPacket.h"
#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace data {

            IbusPacket::IbusPacket(void* packetStart) {
                //Super dangerous constructor that takes a pointer to a byte
                //then increments it to get the length, dangerously reads that
                //many bytes and makes a vector of ints.

                uint8_t* originalPtr = (uint8_t*) packetStart;
                uint8_t len = *(originalPtr + 1);

                if (len == 0 || len > 255) { //TODO Can len > 255 happen irl?
                    //We have a malformed packet. Just return the smallest possible packet.
                    std::vector<uint8_t> bytes = std::vector<uint8_t>();
                    bytes.push_back(0x00);
                    bytes.push_back(00);
                    bytes.push_back(00);
                    bytes.push_back(00);

                    auto newPacketByConstructor = IbusPacket((std::vector<uint8_t>)bytes);
                    cloneFrom(newPacketByConstructor);
                    return;
                } else {
                    //Blindly read len bytes and hope for the best.

                    auto newRaw = std::vector<uint8_t>(len);
                    for(int i = 0; i < len; i++) {
                        newRaw[i] = *(originalPtr + len);
                    }

                    auto newPacketByConstructor = IbusPacket(newRaw);
                    cloneFrom(newPacketByConstructor);
                    return;
                }
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
                for (auto iterator = raw.begin(); iterator != (raw.end() - 1); ++iterator) {
                    //Skip the CRC byte at the end
                    actualCrc = actualCrc ^ *iterator;
                }
            }

            IbusPacket::IbusPacket(IbusDeviceEnum src, IbusDeviceEnum dest, std::vector<uint8_t> data) {

                sourceDevice = src;
                destinationDevice = dest;
                this->data = std::vector<uint8_t>(data);

                this->packetLength = data.size() + 4;

                if (packetLength <= 2) {
                    data = std::vector<uint8_t>();
                } else {
                    this->data = std::move(data);
                }

                //Set complete Raw packet
                completeRawPacket = std::vector<uint8_t>(packetLength);
                completeRawPacket[0] = sourceDevice;
                completeRawPacket[1] = packetLength;
                completeRawPacket[2] = destinationDevice;

                int completeRawPacketIndex = 3;
                for (uint8_t byte: data) {
                    completeRawPacket[completeRawPacketIndex++] = byte;
                }

                //Now calculate the actual CRC.
                actualCrc = 0;
                for (auto iterator = completeRawPacket.begin(); iterator != (completeRawPacket.end() - 1); ++iterator) {
                    //Skip the CRC byte at the end
                    actualCrc = actualCrc ^ *iterator;
                }
                givenCrc = actualCrc;

                completeRawPacket[completeRawPacketIndex] = actualCrc;
            }

            IbusPacket::~IbusPacket() {
                data.clear();
                data.shrink_to_fit();
                completeRawPacket.clear();
                completeRawPacket.shrink_to_fit();
            }

            void IbusPacket::cloneFrom(IbusPacket other) {
                this->completeRawPacket = other.completeRawPacket;
                this->actualCrc = other.actualCrc;
                this->data = other.data;
                this->packetLength = other.packetLength;
                this->destinationDevice = other.destinationDevice;
                this->sourceDevice = other.sourceDevice;
                this->givenCrc = other.givenCrc;
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