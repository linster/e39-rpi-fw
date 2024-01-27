//
// Created by stefan on 12/11/22.
//

#include "PiDefaultConfigurationProvider.h"

namespace pico::config::defaults {

    Configuration PiDefaultConfigurationProvider::getDefaultConfiguration() {
        return Configuration(
                messages::ConfigMessage{
                        .isIbusLogOutputEnabled = false,
                        .enabledMaxLogLevelForIbusLog = logger::BaseLogger::Level::WTF,
                        .enabledMaxLogLevelForPrintfLog = logger::BaseLogger::Level::DEBUG,
                        .alwaysTurnOnRpiOnStatup = true,
                        .alwaysTurnOnScreenOnIbusActivity = true,
                        .scanProgramOnBoot = messages::ConfigMessage::ScanProgram::LINSTEROS_BOOTSPLASH,
                        .videoSourceOnBoot = hardware::videoSwitch::VideoSource::RVC,
                        .sendBMBTEncodingPacketOnBootup = false,
                        .videoEncoding = messages::ConfigMessage::VideoEncoding::NTSC,
                        .aspectRatio = messages::ConfigMessage::AspectRatio::SixteenNine
                });
    }
} // defaults