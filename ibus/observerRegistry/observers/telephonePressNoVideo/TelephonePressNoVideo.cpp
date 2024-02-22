//
// Created by stefan on 2/22/24.
//

#include "TelephonePressNoVideo.h"


namespace pico::ibus::observers {
    TelephonePressNoVideo::TelephonePressNoVideo(
            std::shared_ptr<logger::BaseLogger> baseLogger,
            std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
            std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager,
            std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter,
            std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager) {

        this->logger = baseLogger;
        this->videoSwitch = videoSwitch;
        this->screenPowerManager = screenPowerManager;
        this->testingOutputWriter = testingOutputWriter;
        this->pi4PowerSwitchManager = pi4PowerSwitchManager;
    }

    void TelephonePressNoVideo::restartRpi() {
        pi4PowerSwitchManager->setPower(false);
        sleep_ms(500);
        pi4PowerSwitchManager->setPower(true);
    }

    void TelephonePressNoVideo::swapToNextVideoSource() {


        hardware::videoSwitch::VideoSource previous = this->videoSwitch->getPreviousVideoSource();
        hardware::videoSwitch::VideoSource next;
        switch (previous) {
            case hardware::videoSwitch::RVC:
                next = hardware::videoSwitch::UPSTREAM;
                break;
            case hardware::videoSwitch::UPSTREAM:
            case hardware::videoSwitch::PICO:
                next = hardware::videoSwitch::PI;
                break;
            case hardware::videoSwitch::PI:
                next = hardware::videoSwitch::RVC;
                break;
        }
        this->videoSwitch->switchTo(next);
    }

    void TelephonePressNoVideo::onTelephonePressed() {
        swapToNextVideoSource();
    }

    void TelephonePressNoVideo::onTelephoneLongPressed() {
        restartRpi();
    }

    void TelephonePressNoVideo::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
        if (iBusPacket->getSourceDevice() == data::IbusDeviceEnum::BOARDMONITOR_BUTTONS
            && iBusPacket->getDestinationDevice() == data::IbusDeviceEnum::BROADCAST) {

            if (iBusPacket->getDataLength() >= 2) {

                if (iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 0] == 0x48) {
                    uint8_t command = iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 1];

                    if (command == 0x08) {
                        logger->d(getTag(), "Telephone pressed");
                        onTelephonePressed();
                        return;
                    }


                    if (command == 0x88) {
                        logger->d(getTag(), "Telephone released");
                        return;
                    }

                    if (command == 0x48) {
                        onTelephoneLongPressed();
                        return;
                    }
                }
            }
        }
    }


} // observers