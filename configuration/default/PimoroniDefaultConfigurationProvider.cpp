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
//                        .scanProgramOnBoot = messages::ConfigMessage::ScanProgram::LINSTEROS_BOOTSPLASH,
                        .scanProgramOnBoot = messages::ConfigMessage::ScanProgram::NONE,
                        .videoSourceOnBoot = hardware::videoSwitch::VideoSource::RVC,
                        .sendBMBTEncodingPacketOnBootup = false,
                        .videoEncoding = messages::ConfigMessage::VideoEncoding::NTSC,
                        .aspectRatio = messages::ConfigMessage::AspectRatio::SixteenNine
                });
    }
} // defaults