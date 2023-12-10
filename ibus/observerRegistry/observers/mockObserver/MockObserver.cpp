//
// Created by stefan on 12/2/22.
//

#include "MockObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            MockObserver::MockObserver(std::shared_ptr<pico::logger::BaseLogger> logger) {
                this->logger = logger;
                logger->d("MockObserver", "Constructed");
            }

            void MockObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
                logger->d("MockObserver", iBusPacket->toString());
            }
        } // pico
    } // ibus
} // observers