//
// Created by stefan on 4/5/23.
//

#include "PicoVideoRequestObserver.h"

namespace pico::ibus::observers {

            PicoVideoRequestObserver::PicoVideoRequestObserver(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch) {
                this->logger = logger;
                this->videoSwitch = videoSwitch;
                if (this->videoSwitch == nullptr) {
                    logger->wtf("PicoVideoRequestObserver", "videoSwitch is null");
                }
            }

            void PicoVideoRequestObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
                if (iBusPacket->getSourceDevice() == PI_VALUE && iBusPacket->getDestinationDevice() == PICO_VALUE) {
                    messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, *iBusPacket);
                    onNewPiToPicoPacket(decoded);
                }
            }

            void PicoVideoRequestObserver::onNewPiToPicoPacket(messages::PiToPicoMessage message) {

                switch (message.messageType) {
                    case messages::PiToPicoMessage::MessageType::PicoVideoRequestRVC:
                    case messages::PiToPicoMessage::PicoVideoRequestUpstream:
                    case messages::PiToPicoMessage::PicoVideoRequestPico:
                    case messages::PiToPicoMessage::PicoVideoRequestRpi:
                        logger->d(getTag(), "Got a video source switch request.");
                    default: break;
                }

                switch (message.messageType) {
                    case messages::PiToPicoMessage::MessageType::PicoVideoRequestRVC:
                        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::RVC);
                        break;
                    case messages::PiToPicoMessage::PicoVideoRequestUpstream:
                        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::UPSTREAM);
                        break;
                    case messages::PiToPicoMessage::PicoVideoRequestPico:
                        videoSwitch->switchTo(hardware::videoSwitch::PICO);
                        break;
                    case messages::PiToPicoMessage::PicoVideoRequestRpi:
                        videoSwitch->switchTo(hardware::videoSwitch::PI);
                        break;
                    default: break;
                }
            }
        } // observers