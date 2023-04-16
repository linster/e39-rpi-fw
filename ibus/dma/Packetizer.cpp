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

                if (position == 255) {
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

                packetBytes.push_back(byte);


                if (position == expectedLength + 1) {
                    //We're at the end of where the packet says we should be.
                    if (currentChecksum == expectedChecksum) {
                        packetOk = true;
                    }
                } else {
                    //Update the checksum.
                    currentChecksum = currentChecksum ^ byte;
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
                return packetBytes;
            }

            void Packetizer::recycle() {

                //Lets just grab a copy of the [position..end] bytes,
                //then clear our state, and then add all the bytes again.

                std::vector<uint8_t> subsequentBytes = std::vector<uint8_t>();


                for (auto iter = packetBytes.begin(); iter != packetBytes.end(); ++iter) {
                    iter.
                    if (iter <= position) {
                        break;
                    }
                }



                reset();
                addBytes(subsequentBytes);
            }

            void Packetizer::reset() {
                packetBytes.clear();
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
                          fmt::format("Packetizer{"
                                      "packetBytes (len) %d: ,"
                                      "position: %d, "
                                      "sourceId: %d, "
                                      "destinationId: %d, "
                                      "expectedLength: %d, "
                                      "expectedChecksum: %d, "
                                      "currentChecksum: %d ,"
                                      "packetOk: %d }",
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