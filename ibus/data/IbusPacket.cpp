//
// Created by stefan on 12/2/22.
//

#include "IbusPacket.h"
#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace data {

            [[deprecated("Only used in legacy DmaManager")]]
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


            IbusPacket::IbusPacket(std::array<uint8_t, 255> raw) {

                uint8_t len = raw[1];
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
                    //TODO STEFAN that's the bug. We have to loop over the whole array and not run past it.
//                    std::vector<uint8_t> bytes = std::vector<uint8_t>(raw.begin(), raw.begin() + 2 + len);

                    //We already have the raw bytes for the complete packet, just change the type of the container.
                    std::vector<uint8_t> bytes = std::vector<uint8_t>(raw.begin(), raw.end());

                    //TODO this is really dumb too. We should have a private method that takes in an interable
                    //TODO that sets everything up. CloneFrom isn't smart enough because it doesn't set all the fields.
                    auto newPacketByConstructor = IbusPacket((std::vector<uint8_t>)bytes);
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
                    data = std::vector<uint8_t>(raw.size() - 4);
                    // subtract 2 because length includes checksum and dest address
                    auto begin = raw.begin() + 3;
                    auto end = raw.end() - 1;
                    data.insert(data.begin(), begin, end);
                    //TODO resize data to size so we don't have a gigantic vector mostly full of zeros.
                    //data.resize(packetLength - 2);

                    completeRawPacket.resize(packetLength + 2);
                }

                givenCrc = raw.back();

                //Now calculate the actual CRC.
                actualCrc = 0;
                for (auto iterator = raw.begin(); iterator != (raw.end() - 1); ++iterator) {
                    //Skip the CRC byte at the end
                    actualCrc = actualCrc ^ *iterator;
                }
            }

            //Typically used in building messages to the car.
            IbusPacket::IbusPacket(IbusDeviceEnum src, IbusDeviceEnum dest, std::vector<uint8_t> data) {

                sourceDevice = src;
                destinationDevice = dest;

                this->packetLength = data.size() + 2 /* destDevice + CRC */;

                if (packetLength <= 2) {
                    this->data = std::vector<uint8_t>();
                } else {
                    this->data = std::vector<uint8_t>(data.begin(), data.end());
                }

                //Set complete Raw packet
                completeRawPacket = std::vector<uint8_t>(packetLength + 2);
                completeRawPacket[0] = sourceDevice;
                completeRawPacket[1] = packetLength;
                completeRawPacket[2] = destinationDevice;

                int completeRawPacketIndex = 3;
                for (uint8_t byte: this->data) {
                    completeRawPacket[completeRawPacketIndex++] = byte;
                }

                //Now calculate the actual CRC.
                //TODO STEFAN make sure this works because this constructor won't make valid packets on outgoing (pico origin)
                actualCrc = 0;
                for (auto iterator = completeRawPacket.begin(); iterator != (completeRawPacket.end() - 1); ++iterator) {
                    //Skip the CRC byte at the end
                    actualCrc = actualCrc ^ *iterator;
                }
                givenCrc = actualCrc;

                completeRawPacket[completeRawPacketIndex] = actualCrc;
            }

//            IbusPacket::~IbusPacket() {
//                data.clear();
//                data.shrink_to_fit();
//                completeRawPacket.clear();
//                completeRawPacket.shrink_to_fit();
//            }

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


                std::string sourceDeviceString = IbusDeviceEnumToString(sourceDevice);
                std::string destinationDeviceString = IbusDeviceEnumToString(destinationDevice);

                //TODO do this with a join as it was.
                std::string dataString = std::string();

                for (auto byte : data) {
                    dataString += fmt::format(" {0:#x}", byte);
                }

                std::string rawPacketString = std::string();

                for (auto byte : completeRawPacket) {
                    rawPacketString += fmt::format(" {0:#x}", byte);
                }

                std::string ret = fmt::format("IbusPacket( "
                                   "SourceDevice: {} "
                                   "DestDevice: {} ,"
                                   "Len field: {} ,"
                                   "Data: {} ,"
                                   "RawPacket: {})",
                                   sourceDeviceString,
                                   destinationDeviceString,
                                   packetLength,
                                   dataString,
//                                   fmt::join(completeRawPacket, ",")
                                              rawPacketString
                                   );

                return ret;
            }

            std::vector<uint8_t> IbusPacket::getRawPacket() {
                return completeRawPacket;
            }

            std::string IbusPacket::IbusDeviceEnumToString(IbusDeviceEnum value) {

                std::string ret;
                switch (value) {
                    case BODY_MODULE: ret = BODY_MODULE_STRING; break;
                    case BROADCAST: ret = BROADCAST_STRING; break;
                    case BROADCAST_BF: ret = BROADCAST_BF_STRING; break;
                    case MFL: ret = MFL_STRING; break;
                    case RADIO: ret = RADIO_STRING; break;
                    case MID: ret = MID_STRING; break;
                    case DSP: ret = DSP_STRING; break;
                    case TELEPHONE: ret = TELEPHONE_STRING; break;
                    case IKE: ret = IKE_STRING; break;
                    case IKE_TEXTBAR: ret = IKE_TEXTBAR_STRING; break;
                    case TV_MODULE: ret = TV_MODULE_STRING; break;
                    case NAV_VIDEOMODULE: ret = NAV_VIDEOMODULE_STRING; break;
                    case NAV_MENUSCREEN: ret = NAV_MENUSCREEN_STRING; break;
                    case NAV_LOCATION: ret = NAV_LOCATION_STRING; break;
                    case BOARDMONITOR_BUTTONS: ret = BOARDMONITOR_BUTTONS_STRING; break;
                    case DIS: ret = DIS_STRING; break;
                    case NAVJ: ret = NAVJ_STRING; break;
                    case PICO: ret = PICO_STRING; break;
                    case RPI: ret = RPI_STRING; break;
                    default:
                        ret = fmt::format("{0:#x}", (int)value);
                }
                return ret;
            }

            void IbusPacket::setPacketSource(PacketSource source) {
                this->packetSource = source;
            }

            PacketSource IbusPacket::getPacketSource() {
                return this->packetSource;
            }


        } // pico
    } // ibus
} // data