//
// Created by stefan on 4/15/23.
//

#include "Packetizer.h"

namespace pico::ibus::dma {

            Packetizer::Packetizer() {
                //TODO we can change this to an array since we never resize it.
                //Must have size 255 (not just capacity): addByte() writes packetBytes[position].
                //This used to clear() right after, leaving size 0 until the first reset().
                packetBytes = std::vector<uint8_t>(255, 0);
                position = 0;
                sourceId = 0;
                expectedLength = 0;
                destinationId = 0;
                expectedChecksum = 0;
                currentChecksum = 0;
                packetOk = false;
            }

            void Packetizer::addByte(uint8_t byte) {

//                if (position >= 255) {
//                    reset();
//                    return;
//                }

                if (expectedLength > 0 && position > (expectedLength + 2)) {
                    //The packet can never be right, start over.
                    //Or, the user is adding data without having called recycle after the packet is ok.
                    //We may also want to consider recycling instead of resetting here...
                    //TODO we might have to move this below the first if-statement. Because,
                    //TODO if the packet is invalid, we mutate state along the away?
                    reset();
                    //TODO we need to early-return here because if we don't, we increment position
                    //TODO anyways. WOOPS. That's probably why we always miss the first byte
                    //TODO on every packet that isn't the very first one we ever get.
                    //TODO 11:37am, now what we get is an expected length of BF a lot, so what
                    //TODO we need to do is really figure out our loop invariants.
                    return;
                }

                if (position == 0) {
                    sourceId = byte;
                }

                if (position == 1) {
                    if (byte > MAX_LENGTH_FIELD) {
                        //A 256/257-byte packet can't fit our buffer (or anything downstream).
                        //Treat it as line noise and start over.
                        reset();
                        return;
                    }
                    expectedLength = byte;
                }

                if (position == 2) {
                    destinationId = byte;
                }

                if (position >= packetBytes.size()) {
                    //Shouldn't be reachable now that len is capped and bad checksums reset,
                    //but never write past the buffer.
                    reset();
                    return;
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

                    //Checksum mismatch: this packet can never become valid, so start over now.
                    //(Previously we kept accepting bytes until position > len + 2, which ate
                    //the start of the next packet and, for len = 253, wrote packetBytes[255].)
                    reset();
                    return;
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

                //This used to copy packetBytes[position+1..] and re-add them, but the loop tested
                //`position` instead of `i`, so it read past the end forever. Fixing the loop isn't
                //enough: addByte() stops storing once a packet completes, so nothing real ever
                //sits after `position` (only zeros from reset()), and re-adding those zeros builds
                //bogus "00 00" packets. Equivalent to reset(). Not used by the firmware.
                reset();
            }

            void Packetizer::reset() {
                packetBytes.clear();
                packetBytes.resize(255, 0);
                position = 0;
                sourceId = 0;
                expectedLength = 0;
                destinationId = 0;
                expectedChecksum = 0;

                currentChecksum = 0; //TODO I forgot to clear this field.

                packetOk = false;
            }

            void Packetizer::writeState(
                    std::string tag,
                    std::shared_ptr<logger::BaseLogger> logger) {

                std::string bytesString = std::string();

                bool writeBufferContents = false;

                if (writeBufferContents) {
                    bool isEmpty = true;
                    for (auto byte: packetBytes) {
                        if (byte != 0) { isEmpty = false; }
                        bytesString += fmt::format(" {0:#x}", byte);
                    }

                    if (isEmpty) {
                        bytesString = "<empty>";
                    }
                } else {
                    bytesString = "<disabled>";
                }

                logger->d(tag,
                          fmt::format("Packetizer[ "
                                      "packetBytes: {} ,"
                                      "packetBytes (len): {} ,"
                                      "position: {}, "
                                      "sourceId: {:#x}, "
                                      "destinationId: {:#x}, "
                                      "expectedLength (dec): {}, "
                                      "expectedLength (hex): {:#x}, "
                                      "expectedChecksum: {:#x}, "
                                      "currentChecksum: {:#x} ,"
                                      "packetOk: {} ]",
                                      bytesString,
                                      packetBytes.size(),
                                      position,
                                      sourceId,
                                      destinationId,
                                      expectedLength,
                                      expectedLength,
                                      expectedChecksum,
                                      currentChecksum,
                                      packetOk
                                      )
                          );
            }

        } // dma