//
// Created by stefan on 4/4/23.
//

#ifndef PICOTEMPLATE_PICOTOPI_H
#define PICOTEMPLATE_PICOTOPI_H

#include "ConfigMessage.h"
#include <proto_generated/PicoToPi.pb.h>

namespace pico::messages {

        class PicoToPiMessage {
        public:
            enum MessageType {
                HeartbeatRequest,
                HeartbeatResponse,
                LogStatement,
                ConfigStatusResponse,
                PiSoftPowerRestartX,
                PiSoftPowerRestartPi
            };

            MessageType messageType;

            ConfigMessage existingConfig;
            std::string loggerMessage;

        };

        class PicoToPiMessageConverter : public NanoPb::Converter::UnionMessageConverter<
                PicoToPiMessageConverter,
                PicoToPiMessage,
                ca_stefanm_e39_proto_PicoToPi,
                &ca_stefanm_e39_proto_PicoToPi_msg>
        {
        private:
            class PicoToPiMessageTypeConverter : public NanoPb::Converter::EnumConverter<
                    PicoToPiMessageTypeConverter,
                    PicoToPiMessage::MessageType,
                    ca_stefanm_e39_proto_PicoToPi_MessageType> {
            public:
                static ProtoType encode(const LocalType& local) {
                    switch (local) {
                        case PicoToPiMessage::MessageType::HeartbeatRequest: return ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatRequest;
                        case PicoToPiMessage::MessageType::HeartbeatResponse: return ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatResponse;
                        case PicoToPiMessage::MessageType::LogStatement: return ca_stefanm_e39_proto_PicoToPi_MessageType_LogStatement;
                        case PicoToPiMessage::MessageType::ConfigStatusResponse: return ca_stefanm_e39_proto_PicoToPi_MessageType_ConfigStatusResponse;
                        case PicoToPiMessage::MessageType::PiSoftPowerRestartX: return ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartX;
                        case PicoToPiMessage::MessageType::PiSoftPowerRestartPi: return ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi;
                    }
                }
                static LocalType decode(const ProtoType& proto) {
                    switch (proto) {
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatRequest: return PicoToPiMessage::MessageType::HeartbeatRequest;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_HeartbeatResponse: return PicoToPiMessage::MessageType::HeartbeatResponse;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_LogStatement: return PicoToPiMessage::MessageType::LogStatement;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_ConfigStatusResponse: return PicoToPiMessage::ConfigStatusResponse;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartX: return PicoToPiMessage::MessageType::PiSoftPowerRestartX;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi: return PicoToPiMessage::MessageType::PiSoftPowerRestartPi;
                    }
                }
            };
        public:
            static ProtoType encoderInit(const LocalType& local) {

                ProtoType ret {
                    .messageType = PicoToPiMessageTypeConverter::encoderInit(local.messageType)
                };

                if (local.messageType == PicoToPiMessage::ConfigStatusResponse) {
                    ret.which_body = ca_stefanm_e39_proto_PicoToPi_configMessage_tag;
                    ret.body.configMessage = ConfigMessageConverter::encoderInit(local.existingConfig);
                }

                if (local.messageType == PicoToPiMessage::LogStatement) {
                    ret.which_body = ca_stefanm_e39_proto_PicoToPi_loggerStatement_tag;
                    ret.body.loggerStatement = NanoPb::Converter::StringConverter::encoderInit(local.loggerMessage);
                }

                return ret;
            };
            static ProtoType decoderInit(LocalType& local) {
                return ProtoType{
                    .messageType = PicoToPiMessageTypeConverter::decoderInit(local.messageType)
//                    .cb_body = unionDecoderInit(local)
                };
            };

            static bool unionDecodeCallback(pb_istream_t *stream, const pb_field_t *field, LocalType &local){
//                if (field->tag == ca_stefanm_e39_proto_PicoToPi_messageType_tag) {
//
////                }
//                if (field->tag == ca_stefanm_e39_proto_PicoToPi_configMessage_tag) {
////                    auto* msg = static_cast<ca_stefanm_e39_proto_ConfigProto *>(field->pData);
////                    ConfigMessageConverter::decoderInit()
////                    local.existingConfig = ConfigMessage(msg)
//
//                }
//                if (field->tag == ca_stefanm_e39_proto_PicoToPi_loggerStatement_tag) {
//                    NanoPb::Converter::StringConverter::decoderCallbackInit(field->pData);
//                }

                return false;
            }

            static bool decoderApply(const ProtoType& proto, LocalType& local) {
                local.messageType = PicoToPiMessageTypeConverter::decode(proto.messageType);

                if (local.messageType == PicoToPiMessage::ConfigStatusResponse) {
                    if (proto.which_body == ca_stefanm_e39_proto_PicoToPi_configMessage_tag) {
                        ConfigMessageConverter::decoderApply(proto.body.configMessage, local.existingConfig);
                    }
                }

                if (local.messageType == PicoToPiMessage::LogStatement) {
                    if (proto.which_body == ca_stefanm_e39_proto_PicoToPi_loggerStatement_tag) {
                        NanoPb::Converter::StringConverter::decoderApply(proto.body.loggerStatement, local.loggerMessage);
                    }
                }
                return true;
            }

        };
    } // messages

#endif //PICOTEMPLATE_PICOTOPI_H
