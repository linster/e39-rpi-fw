//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_CONFIGURATION_H
#define PICOTEMPLATE_CONFIGURATION_H

#include "../logging/BaseLogger.h"
#include "../proto_cpp/ConfigMessage.h"

namespace pico {
    namespace config {

        class Configuration {

            //a data class with getters and setters for info.

        private:
            //We'll just mutate this.
            messages::ConfigMessage backingMessage;

        public:

            Configuration(messages::ConfigMessage from);

            messages::ConfigMessage toMessage();

            //If enabled, don't write log messages to USB UART, instead encode them as IBUS messages
            //for the USB UART, and print them to another UART as well.
            bool isIBusLogOutputEnabled();

            bool isLogLevelEnabledIbus(pico::logger::BaseLogger::Level level);
            bool isLogLevelEnabledPrintf(pico::logger::BaseLogger::Level level);

            void enableAllLogLevelsIbus();
            void enableAllLogLevelsPrintf();
            void disableAllLogLevelsIbus();
            void disableAllLogLevelsPrintf();
            void enableLevelsForIbusLogging(pico::logger::BaseLogger::Level level);
            void enableLevelsForPrintFLogging(pico::logger::BaseLogger::Level level);


            //If enabled, send a packet to turn on BMBT screen
            bool getTurnOnScreenWhenIbusActive();
            void setTurnOnScreenWhenIbusActive(bool enabled);

            //If enabled, always turn on the Pi4 power supply when we start up.
            bool getAlwaysTurnOnRpiOnStartup();
            void setAlwaysTurnOnRpiOnStartup(bool enabled);


            //If true, on startup emit a packet to BMBT to set encoding and aspect ratio
            bool getShouldSendBMBTInitializationMessageOnBootup();
            void setShouldSendBMBTInitializationMessageOnBootup(bool value);

            messages::ConfigMessage::ScanProgram getScanProgramOnBootup();
            void setScanProgramOnBootup(messages::ConfigMessage::ScanProgram program);

            messages::ConfigMessage::VideoEncoding getVideoEncoding();
            void setVideoEncoding(messages::ConfigMessage::VideoEncoding encoding);

            messages::ConfigMessage::AspectRatio getAspectRatio();
            void setAspectRatio(messages::ConfigMessage::AspectRatio aspectRatio);

            //Was a change made to this object requiring a sync to all config stores?
            //A flag to indicate the config object is dirty, and that
            //the config manager should repost the backing message to all of its
            //stores.
            bool isSyncRequired = true;

            //To be used only by the configuration manager to indicate the dirty config
            //was synced to all more-permanent stores.
            void clearSyncRequired();

        };

    } // pico
} // config

#endif //PICOTEMPLATE_CONFIGURATION_H
