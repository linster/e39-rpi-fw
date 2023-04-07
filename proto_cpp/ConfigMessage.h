//
// Created by stefan on 4/4/23.
//

#ifndef PICOTEMPLATE_CONFIGMESSAGE_H
#define PICOTEMPLATE_CONFIGMESSAGE_H

#include "../libs/nanopb_cpp/nanopb_cpp.h"
#include "../proto_generated/ConfigProto.pb.h"
#include "../logging/BaseLogger.h"
#include "../hardware/videoSwitch/VideoSource.h"

namespace pico {
    namespace messages {

        class ConfigMessage {

        public:
            bool isIbusLogOutputEnabled;
            //Confusing naming, but max means "max number of log lines" --> minimum severity
            pico::logger::BaseLogger::Level enabledMaxLogLevelForIbusLog;
            pico::logger::BaseLogger::Level enabledMaxLogLevelForPrintfLog;

            bool alwaysTurnOnRpiOnStatup;
            bool alwaysTurnOnScreenOnIbusActivity;

            enum ScanProgram {
                NONE = 0,CLOCK = 1,LINSTEROS_BOOTSPLASH = 2, GOOSE_BOOTSPLASH = 3,
                BMW_BOOTSPLASH = 4, MENU = 5
            };
            ScanProgram scanProgramOnBoot;

            hardware::videoSwitch::VideoSource videoSourceOnBoot;

            bool sendBMBTEncodingPacketOnBootup;
            enum VideoEncoding {NTSC = 0, PAL = 1};
            enum AspectRatio {FourThree = 0, SixteenNine = 1, Zoom = 2};

            VideoEncoding videoEncoding;
            AspectRatio aspectRatio;
        };

        class ConfigMessageConverter : public NanoPb::Converter::MessageConverter<
                ConfigMessageConverter,
                ConfigMessage,
                _ca_stefanm_e39_proto_ConfigProto,
                &ca_stefanm_e39_proto_ConfigProto_msg>
        {
        private:
            class VideoSourceConverter : public NanoPb::Converter::EnumConverter<
                    VideoSourceConverter,
                    hardware::videoSwitch::VideoSource,
                    ca_stefanm_e39_proto_ConfigProto_VideoSource>
            {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case pico::hardware::videoSwitch::VideoSource::PI: return ca_stefanm_e39_proto_ConfigProto_VideoSource_PI;
                        case pico::hardware::videoSwitch::VideoSource::PICO: return ca_stefanm_e39_proto_ConfigProto_VideoSource_PICO;
                        case pico::hardware::videoSwitch::VideoSource::RVC: return ca_stefanm_e39_proto_ConfigProto_VideoSource_RVC;
                        case pico::hardware::videoSwitch::VideoSource::UPSTREAM: return ca_stefanm_e39_proto_ConfigProto_VideoSource_UPSTREAM;
                    }
                    //Should never happen, fall back to RVC.
                    return ca_stefanm_e39_proto_ConfigProto_VideoSource_RVC;
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_ConfigProto_VideoSource_PI: return pico::hardware::videoSwitch::VideoSource::PI;
                        case ca_stefanm_e39_proto_ConfigProto_VideoSource_PICO: return pico::hardware::videoSwitch::VideoSource::PICO;
                        case ca_stefanm_e39_proto_ConfigProto_VideoSource_RVC: return pico::hardware::videoSwitch::VideoSource::RVC;
                        case ca_stefanm_e39_proto_ConfigProto_VideoSource_UPSTREAM: return pico::hardware::videoSwitch::VideoSource::UPSTREAM;
                    }
                }
            };
            class VideoEncodingConverter : public NanoPb::Converter::EnumConverter<
                    VideoEncodingConverter,
                    ConfigMessage::VideoEncoding,
                    ca_stefanm_e39_proto_ConfigProto_VideoEncoding>
            {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case ConfigMessage::VideoEncoding::NTSC: return ca_stefanm_e39_proto_ConfigProto_VideoEncoding_NTSC;
                        case ConfigMessage::VideoEncoding::PAL: return ca_stefanm_e39_proto_ConfigProto_VideoEncoding_PAL;
                    }
                    //Should never happen. My car is set up for NTSC because it has more lines than PAL.
                    return ca_stefanm_e39_proto_ConfigProto_VideoEncoding_NTSC;
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_ConfigProto_VideoEncoding_NTSC: return ConfigMessage::VideoEncoding::NTSC;
                        case ca_stefanm_e39_proto_ConfigProto_VideoEncoding_PAL: return ConfigMessage::VideoEncoding::PAL;
                    }
                    return ConfigMessage::VideoEncoding::NTSC;
                }
            };
            class AspectRatioConverter : public NanoPb::Converter::EnumConverter<
                    AspectRatioConverter,
                    ConfigMessage::AspectRatio,
                    ca_stefanm_e39_proto_ConfigProto_AspectRatio>
            {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case ConfigMessage::AspectRatio::FourThree: return ca_stefanm_e39_proto_ConfigProto_AspectRatio_FourThree;
                        case ConfigMessage::AspectRatio::SixteenNine: return ca_stefanm_e39_proto_ConfigProto_AspectRatio_SixteenNine;
                        case ConfigMessage::AspectRatio::Zoom: return ca_stefanm_e39_proto_ConfigProto_AspectRatio_Zoom;
                    }
                    return ca_stefanm_e39_proto_ConfigProto_AspectRatio_SixteenNine;
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_ConfigProto_AspectRatio_FourThree: return ConfigMessage::AspectRatio::FourThree;
                        case ca_stefanm_e39_proto_ConfigProto_AspectRatio_SixteenNine: return ConfigMessage::AspectRatio::SixteenNine;
                        case ca_stefanm_e39_proto_ConfigProto_AspectRatio_Zoom: return ConfigMessage::AspectRatio::Zoom;
                    }
                }
            };
            class ScanProgramConverter : public NanoPb::Converter::EnumConverter<
                    ScanProgramConverter,
                    ConfigMessage::ScanProgram,
                    ca_stefanm_e39_proto_ConfigProto_ScanProgram> {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case ConfigMessage::ScanProgram::NONE: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_NONE;
                        case ConfigMessage::ScanProgram::CLOCK: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_CLOCK;
                        case ConfigMessage::ScanProgram::LINSTEROS_BOOTSPLASH: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_LINSTEROS_BOOTSPLASH;
                        case ConfigMessage::ScanProgram::GOOSE_BOOTSPLASH: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_GOOSE_BOOTSPLASH;
                        case ConfigMessage::ScanProgram::BMW_BOOTSPLASH: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_BMW_BOOTSPLASH;
                        case ConfigMessage::ScanProgram::MENU: return ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU;
                    }
                    return ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU;
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_NONE: return ConfigMessage::ScanProgram::NONE;
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_CLOCK: return ConfigMessage::ScanProgram::CLOCK;
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_LINSTEROS_BOOTSPLASH: return ConfigMessage::ScanProgram::LINSTEROS_BOOTSPLASH;
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_GOOSE_BOOTSPLASH: return ConfigMessage::ScanProgram::GOOSE_BOOTSPLASH;
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_BMW_BOOTSPLASH: return ConfigMessage::ScanProgram::BMW_BOOTSPLASH;
                        case ca_stefanm_e39_proto_ConfigProto_ScanProgram_MENU: return ConfigMessage::ScanProgram::MENU;
                    }
                    return ConfigMessage::ScanProgram::MENU;
                }
            };
            class LogLevelConverter : public NanoPb::Converter::EnumConverter<
                    LogLevelConverter,
                    logger::BaseLogger::Level,
                    ca_stefanm_e39_proto_ConfigProto_LoggingLevels>
            {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case logger::BaseLogger::Level::DEBUG: return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_DEBUG;
                        case logger::BaseLogger::Level::ERROR: return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_ERROR;
                        case logger::BaseLogger::Level::INFO: return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_INFO;
                        case logger::BaseLogger::Level::WARN: return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WARN;
                        case logger::BaseLogger::Level::WTF: return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WTF;
                    }
                    return ca_stefanm_e39_proto_ConfigProto_LoggingLevels_DEBUG;
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_ConfigProto_LoggingLevels_DEBUG: return logger::BaseLogger::Level::DEBUG;
                        case ca_stefanm_e39_proto_ConfigProto_LoggingLevels_ERROR: return logger::BaseLogger::Level::ERROR;
                        case ca_stefanm_e39_proto_ConfigProto_LoggingLevels_INFO: return logger::BaseLogger::Level::INFO;
                        case ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WARN: return logger::BaseLogger::Level::WARN;
                        case ca_stefanm_e39_proto_ConfigProto_LoggingLevels_WTF: return logger::BaseLogger::Level::WTF;
                    }
                    return logger::BaseLogger::Level::DEBUG;
                }
            };
        public:
            static ProtoType encoderInit(const LocalType& local) {
                return ProtoType {
                    .isIbusLogOutputEnabled = NanoPb::Converter::BoolConverter::encoderInit(local.isIbusLogOutputEnabled),
                    .enabledMaxLogLevelForIbusLog = LogLevelConverter::encoderInit(local.enabledMaxLogLevelForIbusLog),
                    .enabledMaxLogLevelForPrintfLog = LogLevelConverter::encoderInit(local.enabledMaxLogLevelForPrintfLog),
                    .alwaysTurnOnRpiOnStatup = NanoPb::Converter::BoolConverter::encoderInit(local.alwaysTurnOnRpiOnStatup),
                    .scanProgramOnBoot = ScanProgramConverter::encoderInit(local.scanProgramOnBoot),
                    .videoSourceOnBoot = VideoSourceConverter::encoderInit(local.videoSourceOnBoot),
                    .sendBMBTEncodingPacketOnBootup = NanoPb::Converter::BoolConverter::encoderInit(local.sendBMBTEncodingPacketOnBootup),
                    .videoEncoding = VideoEncodingConverter::encoderInit(local.videoEncoding),
                    .aspectRatio = AspectRatioConverter::encoderInit(local.aspectRatio)
                };
            };
            static ProtoType decoderInit(LocalType& local) {
                return ProtoType {
                    .isIbusLogOutputEnabled = NanoPb::Converter::BoolConverter::decoderInit(local.isIbusLogOutputEnabled),
                    .enabledMaxLogLevelForIbusLog = LogLevelConverter::decoderInit(local.enabledMaxLogLevelForIbusLog),
                    .enabledMaxLogLevelForPrintfLog = LogLevelConverter::decoderInit(local.enabledMaxLogLevelForPrintfLog),
                    .alwaysTurnOnRpiOnStatup = NanoPb::Converter::BoolConverter::decoderInit(local.alwaysTurnOnRpiOnStatup),
                    .scanProgramOnBoot = ScanProgramConverter::decoderInit(local.scanProgramOnBoot),
                    .videoSourceOnBoot = VideoSourceConverter::decoderInit(local.videoSourceOnBoot),
                    .sendBMBTEncodingPacketOnBootup = NanoPb::Converter::BoolConverter::decoderInit(local.sendBMBTEncodingPacketOnBootup),
                    .videoEncoding = VideoEncodingConverter::decoderInit(local.videoEncoding),
                    .aspectRatio = AspectRatioConverter::decoderInit(local.aspectRatio)
                };
            };
            static bool decoderApply(const ProtoType& proto, LocalType& local) {
                //TODO?? Should I do something here?
                return true;
            };
        };

    } // pico
} // messages

#endif //PICOTEMPLATE_CONFIGMESSAGE_H
