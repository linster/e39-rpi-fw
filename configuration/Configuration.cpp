//
// Created by stefan on 11/20/22.
//

#include "Configuration.h"

namespace pico::config {

        Configuration::Configuration(messages::ConfigMessage from) {
            this->backingMessage = from;
            this->isSyncRequired = true;
        }

        Configuration::Configuration() {
            //Make an empty configuration
            this->backingMessage = messages::ConfigMessage();
            isSyncRequired = false;
        }

        messages::ConfigMessage Configuration::toMessage() {
            return this->backingMessage;
        }

        bool Configuration::isIBusLogOutputEnabled() {
            return backingMessage.isIbusLogOutputEnabled;
        }

        void Configuration::enableAllLogLevelsIbus() {
            backingMessage.enabledMaxLogLevelForIbusLog = pico::logger::BaseLogger::Level::DEBUG;
            isSyncRequired = true;
        }

        void Configuration::enableAllLogLevelsPrintf() {
            backingMessage.enabledMaxLogLevelForPrintfLog = pico::logger::BaseLogger::Level::DEBUG;
            isSyncRequired = true;
        }
        void Configuration::disableAllLogLevelsIbus() {
            backingMessage.enabledMaxLogLevelForIbusLog = pico::logger::BaseLogger::Level::WTF;
            isSyncRequired = true;
        }
        void Configuration::disableAllLogLevelsPrintf() {
            backingMessage.enabledMaxLogLevelForPrintfLog = pico::logger::BaseLogger::Level::WTF;
            isSyncRequired = true;
        }
        void Configuration::enableLevelsForIbusLogging(pico::logger::BaseLogger::Level level) {
            backingMessage.enabledMaxLogLevelForIbusLog = level;
            isSyncRequired = true;
        }
        void Configuration::enableLevelsForPrintFLogging(pico::logger::BaseLogger::Level level) {
            backingMessage.enabledMaxLogLevelForPrintfLog = level;
            isSyncRequired = true;
        }
        bool Configuration::getTurnOnScreenWhenIbusActive() {
            return backingMessage.alwaysTurnOnScreenOnIbusActivity;
        }
        void Configuration::setTurnOnScreenWhenIbusActive(bool enabled) {
            backingMessage.alwaysTurnOnScreenOnIbusActivity = enabled;
            isSyncRequired = true;
        }
        bool Configuration::getAlwaysTurnOnRpiOnStartup() {
            return backingMessage.alwaysTurnOnRpiOnStatup;
        }
        void Configuration::setAlwaysTurnOnRpiOnStartup(bool enabled) {
            backingMessage.alwaysTurnOnRpiOnStatup = enabled;
            isSyncRequired = true;
        }
        bool Configuration::getShouldSendBMBTInitializationMessageOnBootup() {
            return backingMessage.sendBMBTEncodingPacketOnBootup;
        }
        void Configuration::setShouldSendBMBTInitializationMessageOnBootup(bool value) {
            backingMessage.sendBMBTEncodingPacketOnBootup = value;
            isSyncRequired = true;
        }

        messages::ConfigMessage::ScanProgram Configuration::getScanProgramOnBootup() {
            return backingMessage.scanProgramOnBoot;
        }
        void Configuration::setScanProgramOnBootup(messages::ConfigMessage::ScanProgram program) {
            backingMessage.scanProgramOnBoot = program;
            isSyncRequired = true;
        }

        messages::ConfigMessage::VideoEncoding Configuration::getVideoEncoding() {
            return backingMessage.videoEncoding;
        }
        void Configuration::setVideoEncoding(messages::ConfigMessage::VideoEncoding encoding) {
            backingMessage.videoEncoding = encoding;
            isSyncRequired = true;
        }

        messages::ConfigMessage::AspectRatio Configuration::getAspectRatio() {
            return backingMessage.aspectRatio;
        }

        void Configuration::setAspectRatio(messages::ConfigMessage::AspectRatio aspectRatio) {
            backingMessage.aspectRatio = aspectRatio;
            isSyncRequired = true;
        }

        void Configuration::clearSyncRequired() {
            isSyncRequired = false;
        }



    } // config