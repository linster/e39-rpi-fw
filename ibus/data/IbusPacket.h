//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_IBUSPACKET_H
#define PICOTEMPLATE_IBUSPACKET_H

#include <memory>
#include <vector>
#include <cstddef>
#include "IbusDeviceEnum.h"

namespace pico::ibus::data {

    ///Used by the DMA Manager to label a packet's source so that it
    ///can be forwarded to other UARTS
    enum PacketSource { NOT_SET, FROM_CAR, FROM_PI };

    class IbusPacket : std::enable_shared_from_this<IbusPacket>{

    private:
        //The complete raw packet that the other fields are made of, including CRC.
        std::vector<uint8_t> completeRawPacket = std::vector<uint8_t>();

        IbusDeviceEnum sourceDevice;
        IbusDeviceEnum destinationDevice;

        uint8_t packetLength;
        std::vector<uint8_t> data;
        uint8_t givenCrc;
        uint8_t actualCrc;

        void cloneFrom(IbusPacket other);

        PacketSource packetSource = NOT_SET;
    public:

        IbusPacket(std::array<uint8_t, 255> raw);

        explicit IbusPacket(std::vector<uint8_t> raw);

        explicit IbusPacket(void * packetStart);

        IbusPacket(
                IbusDeviceEnum src,
                IbusDeviceEnum dest,
                std::vector<uint8_t> data
                );

        IbusDeviceEnum getSourceDevice();
        IbusDeviceEnum getDestinationDevice();
        std::shared_ptr<std::vector<uint8_t>> getData();
        //The CRC in the packet when it's assembled from a stream.
        uint8_t getGivenCrc();
        //The CRC in the packet after assembly.
        uint8_t getActualCrc();
        bool isPacketValid();

        std::vector<uint8_t> getRawPacket();

        std::string toString();

        ~IbusPacket() = default;

        static std::string IbusDeviceEnumToString(IbusDeviceEnum value);

        //TODO override the equals to compare the rawPackets.

        void setPacketSource(PacketSource source);
        PacketSource getPacketSource();
    };

} // data

#endif //PICOTEMPLATE_IBUSPACKET_H
