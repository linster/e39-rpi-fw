//
// Created by stefan on 4/15/23.
//

#include "Packetizer.h"

namespace pico {
    namespace ibus {
        namespace dma {

            Packetizer::Packetizer() {
                packetBytes = std::vector<uint8_t>(255);
                position = 0;
                sourceId = 0;
                expectedLength = 0;
                destinationId = 0;
                expectedChecksum = 0;
                currentChecksum = 0;
                packetOk = false;
            }

            void Packetizer::addByte(uint8_t byte) {

                if (position >= 255) {
                    reset();
                    return;
                }

                if (position == 0) {
                    sourceId = byte;
                }

                if (position == 1) {
                    expectedLength = byte;
                }

                if (position == 2) {
                    destinationId = byte;
                }

                packetBytes[position] = byte;

                if (position == expectedLength + 1) {
                    //We're at the end of where the packet says we should be.

                    //We need to update the checksum byte
                    //The last byte is what we're expecting
                    expectedChecksum = byte;

                    //Don't calculate current checksum
                    //(the checksum doesn't include the checksum byte)

                    if (currentChecksum == expectedChecksum) {
                        packetOk = true;
                        return;
                    }
                } else {
                    //Update the checksum.
                    currentChecksum = currentChecksum ^ byte;
                }

                if (expectedLength > 0 && position > (expectedLength + 1)) {
                    //The packet can never be right, start over.
                    //Or, the user is adding data without having called recycle after the packet is ok.
                    //We may also want to consider recycling instead of resetting here...
                    reset();
                }

                position++; //Make sure to increment before we're called again.
            }

            void Packetizer::addBytes(std::vector<uint8_t> bytes) {
                for (const auto &item: bytes) {
                    addByte(item);
                }
            }

            bool Packetizer::isPacketComplete() {
                return packetOk && (position == expectedLength + 1);
            }

            std::vector<uint8_t> Packetizer::getPacketBytes() {
                //TOOO this needs to return a slice of the array from [0..position]
                //TODO also, check expected length and if packet is complete.
                //[0..position) while incomplete
                //[0..expectedLength] when complete

                std::vector<uint8_t> slice;
                if (!packetOk) {
                    slice = std::vector<uint8_t>(
                            packetBytes.begin(),
                            packetBytes.begin() + position);
                } else {
                    slice = std::vector<uint8_t>(
                            packetBytes.begin(),
                            packetBytes.begin() + position + 1);
                }
                return slice;
            }

            void Packetizer::recycle() {

                //Lets just grab a copy of the (position..end] bytes,
                //then clear our state, and then add all the bytes again.
                std::vector<uint8_t> subsequentBytes = std::vector<uint8_t>();
                for (int i = position + 1; position < packetBytes.size(); i++) {
                    subsequentBytes.push_back(packetBytes[i]);
                }

                reset();
                addBytes(subsequentBytes);
            }

            void Packetizer::reset() {
                packetBytes.clear();
                packetBytes.resize(255, 0);
                position = 0;
                sourceId = 0;
                expectedLength = 0;
                destinationId = 0;
                expectedChecksum = 0;
                packetOk = false;
            }

            void Packetizer::writeState(
                    std::string tag,
                    std::shared_ptr<logger::BaseLogger> logger) {
                logger->d(tag,
                          fmt::format("Packetizer[ "
                                      "packetBytes (len): {} ,"
                                      "position: {}, "
                                      "sourceId: {}, "
                                      "destinationId: {}, "
                                      "expectedLength: {}, "
                                      "expectedChecksum: {}, "
                                      "currentChecksum: {} ,"
                                      "packetOk: {} ]",
                                      packetBytes.size(),
                                      position,
                                      sourceId,
                                      destinationId,
                                      expectedLength,
                                      expectedChecksum,
                                      currentChecksum,
                                      packetOk
                                      )
                          );
            }

        } // pico
    } // ibus
} // dma