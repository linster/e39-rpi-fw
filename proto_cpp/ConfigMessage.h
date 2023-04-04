//
// Created by stefan on 4/4/23.
//

#ifndef PICOTEMPLATE_CONFIGMESSAGE_H
#define PICOTEMPLATE_CONFIGMESSAGE_H

#include "nanopb_cpp.h"
#include "../proto_generated/ConfigProto.pb.h"
#include "../logging/BaseLogger.h"
#include "../hardware/videoSwitch/VideoSource.h"

namespace pico {
    namespace messages {

        class ConfigMessage {

        public:
            bool isIbusLogOutputEnabled;
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
        public:
            static ProtoType encoderInit(const LocalType& local) {
                return ProtoType();
            };
            static ProtoType decoderInit(LocalType& local) {

            };
            static bool decoderApply(const ProtoType& proto, LocalType& local) {
                return true;
            };
        };

    } // pico
} // messages

#endif //PICOTEMPLATE_CONFIGMESSAGE_H
