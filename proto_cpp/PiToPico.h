//
// Created by stefan on 4/4/23.
//

#ifndef PICOTEMPLATE_PITOPICO_H
#define PICOTEMPLATE_PITOPICO_H


#include "ConfigMessage.h"
#include <proto_generated/PiToPico.pb.h>

namespace pico {
    namespace messages {

        class PiToPicoMessage {
        public:

            PiToPicoMessage() = default;
            PiToPicoMessage(const PiToPicoMessage&) = default;
            PiToPicoMessage(PiToPicoMessage&&) = default;


            enum MessageType {
                HeartbeatRequest, HeartbeatResponse,
                ConfigStatusRequest, ConfigPush,

                PicoVideoRequestUpstream, PicoVideoRequestPico,
                PicoVideoRequestRpi, PicoVideoRequestRVC,

                PicoPowerRequestOn,
                PicoPowerRequestOff,
            };
            MessageType messageType;
            ConfigMessage newConfig;
        };

        class PiToPicoMessageConverter : public NanoPb::Converter::MessageConverter<
                PiToPicoMessageConverter,
                PiToPicoMessage,
                ca_stefanm_e39_proto_PiToPico,
                &ca_stefanm_e39_proto_PiToPico_msg> {
        private:
            class PiToPicoMessageMessageTypeConverter : public NanoPb::Converter::EnumConverter<
                    PiToPicoMessageMessageTypeConverter,
                    PiToPicoMessage::MessageType,
                    ca_stefanm_e39_proto_PiToPico_MessageType> {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case PiToPicoMessage::MessageType::HeartbeatRequest: return ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatRequest;
                        case PiToPicoMessage::MessageType::HeartbeatResponse: return ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatResponse;
                        case PiToPicoMessage::MessageType::ConfigStatusRequest: return ca_stefanm_e39_proto_PiToPico_MessageType_ConfigStatusRequest;
                        case PiToPicoMessage::MessageType::ConfigPush: return ca_stefanm_e39_proto_PiToPico_MessageType_ConfigPush;
                        case PiToPicoMessage::MessageType::PicoVideoRequestUpstream: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestUpstream;
                        case PiToPicoMessage::MessageType::PicoVideoRequestPico: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestPico;
                        case PiToPicoMessage::MessageType::PicoVideoRequestRpi: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRpi;
                        case PiToPicoMessage::MessageType::PicoVideoRequestRVC: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRVC;
                        case PiToPicoMessage::MessageType::PicoPowerRequestOn: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOn;
                        case PiToPicoMessage::MessageType::PicoPowerRequestOff: return ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOff;
                    }
                  //TODO do we want invalid here?
                };
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatRequest: return PiToPicoMessage::MessageType::HeartbeatRequest;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_HeartbeatResponse: return PiToPicoMessage::MessageType::HeartbeatResponse;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_ConfigStatusRequest: return PiToPicoMessage::MessageType::ConfigStatusRequest;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_ConfigPush: return PiToPicoMessage::MessageType::ConfigPush;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestUpstream: return PiToPicoMessage::MessageType::PicoVideoRequestUpstream;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestPico: return PiToPicoMessage::MessageType::PicoVideoRequestPico;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRpi: return PiToPicoMessage::MessageType::PicoVideoRequestRpi;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoVideoRequestRVC: return PiToPicoMessage::MessageType::PicoVideoRequestRVC;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOn: return PiToPicoMessage::MessageType::PicoPowerRequestOn;
                        case ca_stefanm_e39_proto_PiToPico_MessageType_PicoPowerRequestOff: return PiToPicoMessage::MessageType::PicoPowerRequestOff;
                    }
                    //TODO do we want invalid here??
                }
            };
        public:
            static ProtoType encoderInit(const LocalType& local) {
                return ProtoType {
                    .messageType = PiToPicoMessageMessageTypeConverter::encoderInit(local.messageType),
                    .newConfig = ConfigMessageConverter::encoderInit(local.newConfig)
                };
            };
            static ProtoType decoderInit(LocalType& local) {
                return ProtoType {
                    .messageType = PiToPicoMessageMessageTypeConverter::decoderInit(local.messageType),
                    .newConfig = ConfigMessageConverter::decoderInit(local.newConfig)
                };
            };
            static bool decoderApply(const ProtoType& proto, LocalType& local) {

                local.messageType = PiToPicoMessageMessageTypeConverter::decode(proto.messageType);
                if (proto.has_newConfig) {
                    ConfigMessageConverter::decoderApply(proto.newConfig, local.newConfig);
                }
                return true;
            }
        };

    } // pico
} // messages

#endif //PICOTEMPLATE_PITOPICO_H
