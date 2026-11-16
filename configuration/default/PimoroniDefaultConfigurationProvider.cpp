//
// Created by stefan on 12/11/22.
//

#include "PimoroniDefaultConfigurationProvider.h"

namespace pico::config::defaults {

    Configuration PimoroniDefaultConfigurationProvider::getDefaultConfiguration() {
        return Configuration(
                messages::ConfigMessage{
                        .rpiFwGitCommitHash = "lalalalalalala",
                        .isIbusLogOutputEnabled = false,
                        .enabledMaxLogLevelForIbusLog = logger::BaseLogger::Level::WTF,
                        .enabledMaxLogLevelForPrintfLog = logger::BaseLogger::Level::DEBUG,
                        .alwaysTurnOnRpiOnStatup = true,
                        .alwaysTurnOnScreenOnIbusActivity = true,
                        //Applied at boot by ApplicationContainer::onMain(): the Linster OS bootsplash on
                        //the Pico's own video, as the firmware always did before reading these settings.
                        .scanProgramOnBoot = messages::ConfigMessage::ScanProgram::LINSTEROS_BOOTSPLASH,
                        .videoSourceOnBoot = hardware::videoSwitch::VideoSource::PICO,
                        .sendBMBTEncodingPacketOnBootup = false,
                        .videoEncoding = messages::ConfigMessage::VideoEncoding::NTSC,
                        .aspectRatio = messages::ConfigMessage::AspectRatio::SixteenNine
                });
    }
} // defaults