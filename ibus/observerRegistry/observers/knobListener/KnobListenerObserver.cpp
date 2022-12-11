//
// Created by stefan on 12/2/22.
//

#include "KnobListenerObserver.h"
#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace observers {

            KnobListenerObserver::KnobListenerObserver(std::shared_ptr<logger::BaseLogger> baseLogger) {
                this->logger = baseLogger;
            }

            void KnobListenerObserver::onNewPacket(pico::ibus::data::IbusPacket iBusPacket) {
                if (iBusPacket.getSourceDevice() == data::IbusDeviceEnum::BOARDMONITOR_BUTTONS &&
                 iBusPacket.getDestinationDevice() == data::NAV_VIDEOMODULE) {

                    if (iBusPacket.getData()->size() >= 2) {
                        //PushData = 0x48, 0x05
                        auto pushData = std::vector<uint8_t>(0x48, 0x05);
                        if (*(iBusPacket.getData()) == pushData) {
                            onKnobPressed();
                        }

                        if (*(iBusPacket.getData()->data()) == 0x49) {
                            uint8_t rotation = (*iBusPacket.getData())[1];

                            if (rotation - 0x0 >= 1 && rotation - 0x80 <= 9) {
                                onKnobTurnedLeft(rotation);
                            }

                            if (rotation - 0x80 >= 1 && rotation - 0x80 <= 9) {
                                onKnobTurnedRight(rotation - 0x80);
                            }
                        }
                    }
                }
            }

            void KnobListenerObserver::onKnobTurnedLeft(int clicks) {
                logger->d(getTag(), fmt::format("onKnobTurnedLeft, clicks {:d}", clicks));
            }

            void KnobListenerObserver::onKnobTurnedRight(int clicks) {
                logger->d(getTag(), fmt::format("onKnobTurnedRight, clicks {:d}", clicks));
            }

            void KnobListenerObserver::onKnobPressed() {
                logger->d(getTag(), "onKnobPressed");
            }


        } // pico
    } // ibus
} // observers