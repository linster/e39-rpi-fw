//
// Created by stefan on 12/10/22.
//

#include "TelephoneLongPressObserver.h"

namespace pico::ibus::observers {

            TelephoneLongPressObserver::TelephoneLongPressObserver(
                    std::shared_ptr<logger::BaseLogger> baseLogger,
                    std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                    std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager,
                    std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter) {
                this->logger = baseLogger;
                this->videoSwitch = videoSwitch;
                if (this->videoSwitch == nullptr) {
                    logger->wtf("TelephoneLongPressObserver", "videoSwitch is null");
                }

                this->scanProgramSwapper = scanProgramSwapper;
                if (this->scanProgramSwapper == nullptr) {
                    logger->wtf("TelephoneLongPressObserver", "scanProgramSwapper is null");
                }

                this->screenPowerManager = screenPowerManager;
                if (this->screenPowerManager == nullptr) {
                    logger->wtf("TelephoneLongPressObserver", "screenPowerManager is null");
                }

                this->testingOutputWriter = testingOutputWriter;
                if (this->testingOutputWriter == nullptr) {
                    logger->wtf("TelephoneLongPressObserver", "testingOutputWriter is null");
                }

            }

            void TelephoneLongPressObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
                if (iBusPacket->getSourceDevice() == data::IbusDeviceEnum::BOARDMONITOR_BUTTONS
                    && iBusPacket->getDestinationDevice() == data::IbusDeviceEnum::BROADCAST) {

                    if (iBusPacket->getDataLength() >= 2) {

                        if (iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 0] == 0x48) {
                            uint8_t command = iBusPacket->getRawPacket()[data::IbusPacket::DATA_START + 1];

                            if (command == 0x08) {
                                logger->d(getTag(), "Telephone pressed");
                                testingOutputWriter->sendDummyPacket();
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
        } // observers