//
// Created by stefan on 12/11/22.
//

#include "ScreenPowerManager.h"
namespace pico::ibus::output::writer {

    ScreenPowerManager::ScreenPowerManager(
            std::shared_ptr<config::defaults::PimoroniDefaultConfigurationProvider> defaultConfigurationProvider,
            std::shared_ptr<logger::BaseLogger> baseLogger,
            std::shared_ptr<dma::IDmaManager> dmaManager) {

        this->logger = baseLogger;
        this->defaultConfiguration = std::make_shared<config::Configuration>(defaultConfigurationProvider->getDefaultConfiguration());
        this->dmaManager = dmaManager;

        aspectRatioIs16_9 = defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::SixteenNine;
        aspectRatioIs4_3 = defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::FourThree;

        encodingIsNtsc = defaultConfiguration->getVideoEncoding() == messages::ConfigMessage::VideoEncoding::NTSC;
        encodingIsPal = !defaultConfiguration->getVideoEncoding() == messages::ConfigMessage::VideoEncoding::PAL;

        logger->d("ScreenPowerManager", "Setting isPowerOn to false.");
        isPowerOn = false;
    }

    std::string ScreenPowerManager::getTag() { return "ScreenPowerManager" ; };

    std::shared_ptr<dma::IDmaManager> ScreenPowerManager::getDmaManager() {
        return dmaManager;
    }

    void ScreenPowerManager::setScreenPowerStateFromObservedIbusMessage(bool isOn) {
        //TODO this needs to be hooked up to the ObserverRegistry... maybe need a separate observer for this?
        logCurrentState("setScreenPowerStateFromObservedIbusMessage");
        logNewState("setScreenPowerStateFromObservedIbusMessage", isOn);

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

        if (defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::SixteenNine ||
                defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::FourThree
        ) {
            uint8_t secondByte = 0x0;
            if (defaultConfiguration->getVideoEncoding() == messages::ConfigMessage::VideoEncoding::NTSC) {
                secondByte |= ENCODING_NTSC;
            } else {
                secondByte |= ENCODING_PAL;
            }

            if (defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::FourThree) {
                secondByte |= ASPECT_4_3;
            }

            if (defaultConfiguration->getAspectRatio() == messages::ConfigMessage::AspectRatio::SixteenNine) {
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
        logCurrentState("sendScreenPowerMessage");
        logNewState("sendScreenPowerMessage", isOn);


        this->isPowerOn = true;

        //TODO STEFAN this function crashes the app.

        auto data = std::vector<uint8_t>(2);
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

    void ScreenPowerManager::logCurrentState(std::string subTag) {
        if (this->isPowerOn) {
            logger->d(getTag(), fmt::format("{}. Current state is: true", subTag));
        } else {
            logger->d(getTag(), fmt::format("{}. Current state is: false", subTag));
        }
    }

    void ScreenPowerManager::logNewState(std::string subTag, bool newState) {
        if (newState) {
            logger->d(getTag(), fmt::format("{}, new state is: true", subTag));
        } else {
            logger->d(getTag(), fmt::format("{}, new state is: false", subTag));
        }
    }


} // writer