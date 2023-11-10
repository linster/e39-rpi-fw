//
// Created by stefan on 12/10/22.
//

#include "TelephoneLongPressObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            TelephoneLongPressObserver::TelephoneLongPressObserver(
                    std::shared_ptr<logger::BaseLogger> baseLogger,
                    std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                    std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager) {
                this->logger = baseLogger;
                this->videoSwitch = videoSwitch;
                this->scanProgramSwapper = scanProgramSwapper;
                this->screenPowerManager = screenPowerManager;
            }

            void TelephoneLongPressObserver::onNewPacket(pico::ibus::data::IbusPacket iBusPacket) {
                if (iBusPacket.getSourceDevice() == data::IbusDeviceEnum::BOARDMONITOR_BUTTONS
                    && iBusPacket.getDestinationDevice() == data::IbusDeviceEnum::BROADCAST) {

                    if (iBusPacket.getData()->size() >= 2) {
                        logger->d(getTag(), "wat3");
                        logger->d(getTag(), iBusPacket.toString());

                        if ((*iBusPacket.getData())[0] == 0x48) {
                            uint8_t command = (*iBusPacket.getData())[1];

                            if (command == 0x08) {
                                logger->d(getTag(), "Telephone pressed");
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

            void TelephoneLongPressObserver::onTelephoneLongPressed() {
                logger->d(getTag(), "onTelephoneLongPressed()");

                scanProgramSwapper->swapTo(ScanProgram::MENU);
                videoSwitch->switchTo(hardware::videoSwitch::PICO);
                screenPowerManager->sendScreenPowerMessage(true);
            }
        } // pico
    } // ibus
} // observers