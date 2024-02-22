//
// Created by stefan on 12/2/22.
//

#include "IbusPacket.h"
#include "fmt/format.h"

namespace pico::ibus::data {

    IbusPacket::IbusPacket(std::array<uint8_t, 255> raw) {

        uint8_t len = raw[1]; //Always a safe access because we get passed in a fixed size array.
        if (len == 0 || len > 255) { //TODO Can len > 255 happen irl?
            populateEmptyPacket();
            return;
        } else {

            sourceDevice = static_cast<IbusDeviceEnum>(raw[0]);
            packetLength = len;
            destinationDevice = static_cast<IbusDeviceEnum>(raw[2]);

            completeRawPacket = std::vector<uint8_t>(raw.begin(), raw.begin() + packetLength + 2);

            givenCrc = raw[packetLength + 2];
            calculateActualCrc();
            return;
        }
    }

    IbusPacket::IbusPacket(std::unique_ptr<std::array<uint8_t, 255>> raw) {

        uint8_t len = (*raw)[1]; //Always a safe access because we get passed in a fixed size array.
        if (len == 0 || len > 255) { //TODO Can len > 255 happen irl?
            populateEmptyPacket();
            return;
        } else {

            sourceDevice = static_cast<IbusDeviceEnum>((*raw)[0]);
            packetLength = (*raw)[1]; //TODO this should never be bigger than raw.length()
            destinationDevice = static_cast<IbusDeviceEnum>((*raw)[2]);

            completeRawPacket = std::vector<uint8_t>(raw->begin(), raw->begin() + packetLength + 2);

            givenCrc = (*raw)[packetLength + 2];
            calculateActualCrc();
            return;
        }
    }

    IbusPacket::IbusPacket(std::vector<uint8_t> raw) {

        if (raw.size() < 4) {
            populateEmptyPacket();
            return;
        }

        sourceDevice = static_cast<IbusDeviceEnum>(raw[0]);
        packetLength = raw[1]; //TODO this should never be bigger than raw.length()
        destinationDevice = static_cast<IbusDeviceEnum>(raw[2]);

        completeRawPacket = raw;

        completeRawPacket = std::vector<uint8_t>(raw.begin(), raw.end());

        givenCrc = raw.back();

        calculateActualCrc();
    }

    //Typically used in building messages to the car.
    IbusPacket::IbusPacket(IbusDeviceEnum src, IbusDeviceEnum dest, std::vector<uint8_t> data) {

        sourceDevice = src;
        destinationDevice = dest;

        //This is the len field in the packet
        this->packetLength = data.size() + 2 /* destDevice + CRC */;

        //Set complete Raw packet
        completeRawPacket = std::vector<uint8_t>(data.size() + 4 /* source, len, dest, crc */);
        completeRawPacket[0] = sourceDevice;
        completeRawPacket[1] = data.size() + 2;
        completeRawPacket[2] = destinationDevice;

        int completeRawPacketIndex = 3;
        for (uint8_t byte: data) {
            completeRawPacket[completeRawPacketIndex++] = byte;
        }

        calculateActualCrc();
        givenCrc = actualCrc;

        completeRawPacket[completeRawPacketIndex] = actualCrc;
    }

//            IbusPacket::~IbusPacket() {
//                data.clear();
//                data.shrink_to_fit();
//                completeRawPacket.clear();
//                completeRawPacket.shrink_to_fit();
//            }


    void IbusPacket::populateEmptyPacket() {
        completeRawPacket = std::vector<uint8_t>(4);
        completeRawPacket[0] = 0x00;
        completeRawPacket[1] = 0x00;
        completeRawPacket[2] = 0x00;
        completeRawPacket[3] = 0x00;

        sourceDevice = IbusDeviceEnum::BODY_MODULE; //0x0
        destinationDevice = IbusDeviceEnum::BODY_MODULE; //0x0
        packetLength = 2;
        givenCrc = 0x0;
        actualCrc = 0x0;
    }

    void IbusPacket::calculateActualCrc() {
        actualCrc = 0;
        for (auto iterator = completeRawPacket.begin(); iterator != (completeRawPacket.end() - 1); ++iterator) {
            //Skip the CRC byte at the end
            actualCrc = actualCrc ^ *iterator;
        }
    }

    IbusDeviceEnum IbusPacket::getSourceDevice() {
        return sourceDevice;
    }

    IbusDeviceEnum IbusPacket::getDestinationDevice() {
        return destinationDevice;
    }

    uint8_t IbusPacket::getDataLength() {
        return completeRawPacket.size() - 4;
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


        //TODO this is a lot of allocations....
        std::string rawPacketString = std::string();

        for (auto byte: completeRawPacket) {
            rawPacketString += fmt::format(" {0:#x}", byte);
        }

        std::string ret = fmt::format("IbusPacket( "
                                      "SourceDevice: {} "
                                      "DestDevice: {} ,"
                                      "Len field: {} ,"
                                      "RawPacket: {})",
                                      sourceDeviceString,
                                      destinationDeviceString,
                                      packetLength,
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
            case BODY_MODULE:
                ret = BODY_MODULE_STRING;
                break;
            case BROADCAST:
                ret = BROADCAST_STRING;
                break;
            case BROADCAST_BF:
                ret = BROADCAST_BF_STRING;
                break;
            case MFL:
                ret = MFL_STRING;
                break;
            case RADIO:
                ret = RADIO_STRING;
                break;
            case MID:
                ret = MID_STRING;
                break;
            case DSP:
                ret = DSP_STRING;
                break;
            case TELEPHONE:
                ret = TELEPHONE_STRING;
                break;
            case IKE:
                ret = IKE_STRING;
                break;
            case IKE_TEXTBAR:
                ret = IKE_TEXTBAR_STRING;
                break;
            case TV_MODULE:
                ret = TV_MODULE_STRING;
                break;
            case NAV_VIDEOMODULE:
                ret = NAV_VIDEOMODULE_STRING;
                break;
            case NAV_MENUSCREEN:
                ret = NAV_MENUSCREEN_STRING;
                break;
            case NAV_LOCATION:
                ret = NAV_LOCATION_STRING;
                break;
            case BOARDMONITOR_BUTTONS:
                ret = BOARDMONITOR_BUTTONS_STRING;
                break;
            case DIS:
                ret = DIS_STRING;
                break;
            case NAVJ:
                ret = NAVJ_STRING;
                break;
            case PICO:
                ret = PICO_STRING;
                break;
            case RPI:
                ret = RPI_STRING;
                break;
            default:
                ret = fmt::format("{0:#x}", (int) value);
        }
        return ret;
    }

    void IbusPacket::setPacketSource(PacketSource source) {
        this->packetSource = source;
    }

    PacketSource IbusPacket::getPacketSource() {
        return this->packetSource;
    }


} // data