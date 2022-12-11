//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_CONFIGURATION_H
#define PICOTEMPLATE_CONFIGURATION_H

#include "../logging/BaseLogger.h"

namespace pico {
    namespace config {

        class Configuration {

            //a data class with getters and setters for info.


        public:
            //If enabled, don't write log messages to USB UART, instead encode them as IBUS messages
            //for the USB UART, and print them to another UART as well.
            bool isIBusLogOutputEnabled();

            bool isLogLevelEnabled(pico::logger::BaseLogger::Level level);

            void enableAllLogLevels();
            void disableAllLogLevels();
            void enableLogLevel(pico::logger::BaseLogger::Level level, bool enabled);


            //If enabled, send a packet to turn on BMBT screen
            bool getTurnOnScreenWhenIbusActive();
            void setTurnOnScreenWhenIbusActive(bool enabled);

            //If enabled, always turn on the Pi4 power supply when we start up.
            void getAlwaysTurnOnRpiOnStartup();
            void setAlwaysTurnOnRpiOnStartup(bool enabled);

            //If enabled, when we turn on the screen, turn on the clock scan program.
            //This simulates the TV Module / MK4 single line clock at the bottom of the screen.
            void getTurnOnClockScanProgramOnIbusActive();
            void setTurnOnClockScanProgramOnIbusActive(bool enabled);

            //If enabled, when we turn on the screen, turn on the bootsplash scan program.
            //This shows a pleasant boostplash while the rpi is booting up.
            void getTurnOnBootsplashScanProgramOnIbusActive();
            void setTurnOnBootsplashScanProgramOnIbusActive(bool enabled);

            //If enabled, when we turn on the screen, switch the video input to the rpi
            //right away so we can see it boot.
            void getTurnOnRpiVideoSourceOnIBusActive();
            void setTurnOnRpiVideoSourceOnIBusActive(bool enabled);

            //If true, use the VGA Timing for BMBT, else pick a default for the Pimoroni
            //test board.
            bool getIsVgaTimingForBMBT();
            void setIsVgaTimingForBMBT(bool value);

            //Was a change made to this object requiring a sync to all config stores?
            bool isSyncRequired();
            //To be used only by the configuration manager to indicate the dirty config
            //was synced to all more-permanent stores.
            void clearSyncRequired();

        };

    } // pico
} // config

#endif //PICOTEMPLATE_CONFIGURATION_H
