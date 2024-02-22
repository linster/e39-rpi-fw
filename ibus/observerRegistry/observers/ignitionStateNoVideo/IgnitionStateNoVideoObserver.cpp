//
// Created by stefan on 2/22/24.
//

#include "IgnitionStateNoVideoObserver.h"

namespace pico::ibus::observers {
    IgnitionStateNoVideoObserver::IgnitionStateNoVideoObserver(
            std::shared_ptr<logger::BaseLogger> baseLogger,
            std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
            std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
            std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager) {

        this->logger = baseLogger;
        this->pi4PowerSwitchManager = pi4PowerSwitchManager;
        this->videoSwitch = videoSwitch;
        this->screenPowerManager = screenPowerManager;
    }

    void IgnitionStateNoVideoObserver::onIgnitionKeyPosition(int position) {
        logger->d(getTag(), fmt::format("onIgnitionKeyPosition() position {:d}", position));
        switch (position) {
            case 0:
                //Turn the pi off
                // (but this is a hard-shutdown, so the pi also needs to start shutting down on position 1)
                //TODO maybe put a delay here?
                pi4PowerSwitchManager->setPower(false);

                videoSwitch->switchTo(hardware::videoSwitch::VideoSource::UPSTREAM);
                screenPowerManager->sendScreenPowerMessage(false);
                break;
            case 1:
                //Turn the pi on
                pi4PowerSwitchManager->setPower(true);
                videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PI);
                break;
            case 2:
                //The user is in a hurry to bootup, so show them the pi loading.
                videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PI);
                break;
        }
    }

    void IgnitionStateNoVideoObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {

        //https://github.com/piersholt/wilhelm-docs/blob/master/ike/11.md
        //80 04 BF 11 00 2A   # KL-30  -- position 0
        //80 04 BF 11 01 2B   # KL-R   -- position 1
        //80 04 BF 11 03 29   # KL-15  -- position 2
        //80 04 BF 11 07 2D   # KL-50  -- position 3


        if (iBusPacket->getSourceDevice() == data::IbusDeviceEnum::IKE && iBusPacket->getDestinationDevice() == 0xBF) {

            if (iBusPacket->getDataLength() >= 2) {
                if (iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 0] == 0x11) {
                    uint8_t position = iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 1];

                    if (ignoreFutureRealIgnitionEmissions) {
                        logger->d(getTag(), fmt::format("Ignoring real ignition status with position %d", position));
                    } else {
                        switch (position) {
                            case 0x00:
                                onIgnitionKeyPosition(0);
                                break;
                            case 0x01:
                                onIgnitionKeyPosition(1);
                                break;
                            case 0x03:
                                onIgnitionKeyPosition(2);
                                break;
                            case 0x07:
                                onIgnitionKeyPosition(3);
                                break;
                            default: break;
                        }
                    }
                }
            }
        }

        /*
         * Also support simulated ignition key.
         *
         * On the test bench, with only an IKE, and no GM or LCM, the IKE emits a repeated ignition key 0 when
         * the switched power (fuse 30?) is on. I tried to inject a message with key position 1, but what
         * happens is that it's spammed out from the IKE. Here, we listen for simulated ignition key events,
         * then set a flag to ignore real ones until the pico restarts.
         */

        if (iBusPacket->getSourceDevice() == PI_VALUE && iBusPacket->getDestinationDevice() == PICO_VALUE) {
            messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, *iBusPacket);
            if (decoded.messageType == messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition0 ||
                decoded.messageType == messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition1 ||
                decoded.messageType == messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition2 ||
                decoded.messageType == messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition3) {

                logger->i(getTag(), "Received a simulated ignition key message");
                ignoreFutureRealIgnitionEmissions = true;

                switch (decoded.messageType) {
                    case messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition0:
                        onIgnitionKeyPosition(0);
                        break;
                    case messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition1:
                        onIgnitionKeyPosition(1);
                        break;
                    case messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition2:
                        onIgnitionKeyPosition(2);
                        break;
                    case messages::PiToPicoMessage::MessageType::SimulatedIgnitionPosition3:
                        onIgnitionKeyPosition(3);
                        break;
                    default: break;
                }
            }
        }

    }


} // observers