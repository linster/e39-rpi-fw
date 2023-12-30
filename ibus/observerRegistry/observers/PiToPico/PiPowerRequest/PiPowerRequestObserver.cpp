//
// Created by stefan on 4/5/23.
//

#include "PiPowerRequestObserver.h"

namespace pico::ibus::observers {

            PiPowerRequestObserver::PiPowerRequestObserver(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> powerSwitchManager) {
                this->logger = logger;
                this->powerSwitchManager = powerSwitchManager;
                if (this->powerSwitchManager == nullptr) {
                    logger->wtf("PiPowerRequestObserver", "powerSwitchManager is null");
                }
            }

            void PiPowerRequestObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
                if (iBusPacket->getSourceDevice() == PI_VALUE && iBusPacket->getDestinationDevice() == PICO_VALUE) {
                    messages::PiToPicoMessage decoded = decodePiToPicoMessage(logger, *iBusPacket);
                    onNewPiToPicoPacket(decoded);
                }
            }

            void PiPowerRequestObserver::onNewPiToPicoPacket(messages::PiToPicoMessage message) {
                if (message.messageType == messages::PiToPicoMessage::MessageType::PicoPowerRequestOff) {
                    logger->d(getTag(), "Got a pi powerOff request.");
                    powerSwitchManager->setPower(false);
                }
                if (message.messageType == messages::PiToPicoMessage::MessageType::PicoPowerRequestOn) {
                    logger->d(getTag(), "Got a pi powerOn request");
                    powerSwitchManager->setPower(true);
                }
            }

        } // observers