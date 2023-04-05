//
// Created by stefan on 4/4/23.
//

#ifndef PICOTEMPLATE_PICOTOPI_H
#define PICOTEMPLATE_PICOTOPI_H

#include "ConfigMessage.h"
#include "../proto_generated/PicoToPi.pb.h"

namespace pico {
    namespace messages {

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
            std::string loggerStatement;
        };

        class PicoToPiMessageConverter : public NanoPb::Converter::MessageConverter<
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
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartX: return PicoToPiMessage::MessageType::PiSoftPowerRestartX;
                        case ca_stefanm_e39_proto_PicoToPi_MessageType_PiSoftPowerRestartPi: return PicoToPiMessage::MessageType::PiSoftPowerRestartPi;
                    }
                }
            };
        public:
            static ProtoType encoderInit(const LocalType& local) {
                return ProtoType {
                    .messageType = PicoToPiMessageTypeConverter::encoderInit(local.messageType),
                    .existingConfig = ConfigMessageConverter::encoderInit(local.existingConfig),
                    .loggerStatement = NanoPb::Converter::StringConverter::encoderInit(local.loggerStatement)
                };
            };
            static ProtoType decoderInit(LocalType& local) {
                return ProtoType {
                    .messageType = PicoToPiMessageTypeConverter::decoderInit(local.messageType),
                    .existingConfig = ConfigMessageConverter::decoderInit(local.existingConfig),
                    .loggerStatement = NanoPb::Converter::StringConverter::decoderInit(local.loggerStatement)
                };
            };
        };
    } // pico
} // messages

#endif //PICOTEMPLATE_PICOTOPI_H
