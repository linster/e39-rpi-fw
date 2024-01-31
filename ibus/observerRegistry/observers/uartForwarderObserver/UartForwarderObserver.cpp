//
// Created by stefan on 1/2/24.
//

#include "UartForwarderObserver.h"

namespace pico::ibus::observers {
    UartForwarderObserver::UartForwarderObserver(
            std::shared_ptr<pico::logger::BaseLogger> logger,
            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager,
            std::shared_ptr<pico::ibus::output::writer::UartForwarderWriter> writer,
            std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch) {
        this->logger = logger;
        this->busTopologyManager = busTopologyManager;
        this->writer = writer;
        this->videoSwitch = videoSwitch;
    }

    void UartForwarderObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
            return;
        }

        switch (iBusPacket->getPacketSource()) {
            case data::NOT_SET:
                logger->wtf(getTag(), "We observed a packet that did not have a source, not forwarding");
                logger->wtf(getTag(), iBusPacket->toString());
                break;
            case data::FROM_CAR:
                if (shouldForwardPacketToCar(iBusPacket)) {
                    writer->forwardPacketToPi(*iBusPacket);
                }
                break;
            case data::FROM_PI:
                writer->forwardPacketToCar(*iBusPacket);
                break;
        }
    }

    bool UartForwarderObserver::shouldForwardPacketToCar(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        //Don't forward the packet if it's a knob turn, knob click, and the video source != rpi.
        bool isKnobTurnOrClick = false;
        if (iBusPacket->getSourceDevice() == data::IbusDeviceEnum::BOARDMONITOR_BUTTONS &&
            iBusPacket->getDestinationDevice() == data::NAV_VIDEOMODULE) {

            if (iBusPacket->getData()->size() >= 2) {
                //PushData = 0x48, 0x05
                if ((*iBusPacket->getData())[0] == 0x48) {
                    if ((*iBusPacket->getData())[1] == 0x05) {
                        isKnobTurnOrClick = true;
                    }
                }
                if ((*iBusPacket->getData())[0] == 0x49) {
                    uint8_t rotation = (*iBusPacket->getData())[1];
                    if (rotation - 0x0 >= 1 && rotation - 0x00 <= 9) {
                        isKnobTurnOrClick = true;
                    }

                    if (rotation - 0x80 >= 1 && rotation - 0x80 <= 9) {
                        isKnobTurnOrClick = true;
                    }
                }
            }
        }

        if (isKnobTurnOrClick && videoSwitch->getPreviousVideoSource() != hardware::videoSwitch::VideoSource::PI) {
            return false;
        }

        return true;
    }
} // observers