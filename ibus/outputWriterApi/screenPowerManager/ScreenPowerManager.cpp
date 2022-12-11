//
// Created by stefan on 12/11/22.
//

#include "ScreenPowerManager.h"
#include "../../../libs/fmt/include/fmt/format.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                ScreenPowerManager::ScreenPowerManager(
                        std::shared_ptr<pico::config::Configuration> defaultConfiguration,
                        std::shared_ptr<logger::BaseLogger> baseLogger) {

                    this->logger = baseLogger;
                    this->defaultConfiguration = defaultConfiguration;

                    aspectRatioIs16_9 = defaultConfiguration->getBMBTAspectRatioIs16_9();
                    aspectRatioIs4_3 = defaultConfiguration->getBMBTAspectRatioIs4_3();

                    encodingIsNtsc = defaultConfiguration->getBMBTEncodingIsNTSC();
                    encodingIsPal = !defaultConfiguration->getBMBTEncodingIsNTSC();

                    logger->d("ScreenPowerManager", "Setting isPowerOn to false.");
                    isPowerOn = false;
                    logger->d("ScreenPowerManager", "Constructed.");
                }

                std::string ScreenPowerManager::getTag() { return "ScreenPowerManager" ; };

                void ScreenPowerManager::setScreenPowerStateFromObservedIbusMessage(bool isOn) {
                    logger->d(getTag(), fmt::format("setScreenPowerStateFromObservedIbusMessage. Current state is: {:b}", this->isPowerOn));
                    logger->d(getTag(), fmt::format("setScreenPowerStateFromObservedIbusMessage, new state is: {:b}", isOn));

                    this->isPowerOn = isOn;
                }

                void ScreenPowerManager::sendScreenInitializationMessage() {
                    logger->d(getTag(), "Sending screen initialization message");

                    this->isPowerOn = true;

                    auto data = std::vector<uint8_t>(3);
                    data.push_back(0x4F);
                    data.push_back(0x10);

                    auto ASPECT_4_3    = 0x00;
                    auto ASPECT_16_9   = 0x10; //0b0001_0000
                    auto ASPECT_ZOOM   = 0x30; //0b0011_0000
                    auto ENCODING_NTSC = 0x01; //0b0000_0001
                    auto ENCODING_PAL  = 0x03; //0b0000_0010

                    if (defaultConfiguration->getBMBTAspectRatioIs16_9() ||
                        defaultConfiguration->getBMBTAspectRatioIs4_3()
                    ) {
                        uint8_t secondByte = 0x0;
                        if (defaultConfiguration->getBMBTEncodingIsNTSC()) {
                            secondByte |= ENCODING_NTSC;
                        } else {
                            secondByte |= ENCODING_PAL;
                        }

                        if (defaultConfiguration->getBMBTAspectRatioIs4_3()) {
                            secondByte |= ASPECT_4_3;
                        }

                        if (defaultConfiguration->getBMBTAspectRatioIs16_9()) {
                            secondByte |= ASPECT_16_9;
                        }
                        data.push_back(secondByte);
                    } else {
                        logger->w(getTag(), "Missing required defaults.");
                    }


                    auto iBusPacket = data::IbusPacket(
                            data::IbusDeviceEnum::PICO,
                            data::IbusDeviceEnum::BOARDMONITOR_BUTTONS,
                            data
                    );

                    this->schedulePacketForWrite(iBusPacket);
                }

                void ScreenPowerManager::sendScreenPowerMessage(bool isOn) {
                    logger->d(getTag(), fmt::format("sendScreenPowerMessage, current state is: {:b}", this->isPowerOn));
                    logger->d(getTag(), fmt::format("sendScreenPowerMessage, new state is: {:b}", isOn));

                    this->isPowerOn = true;

                    auto data = std::vector<uint8_t>(3);
                    data.push_back(0x4F);

                    if (isOn) {
                        data.push_back(0x10);
                    } else {
                        data.push_back(0x00);
                    }

                    auto iBusPacket = data::IbusPacket(
                            data::IbusDeviceEnum::PICO,
                            data::IbusDeviceEnum::BOARDMONITOR_BUTTONS,
                            data
                            );

                    this->schedulePacketForWrite(iBusPacket);
                }


            } // pico
        } // ibus
    } // output
} // writer