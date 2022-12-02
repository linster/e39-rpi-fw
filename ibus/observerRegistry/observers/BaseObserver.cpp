//
// Created by stefan on 12/2/22.
//

#include "BaseObserver.h"
#include "../../../libs/fmt/include/fmt/format.h"

namespace pico {
    namespace ibus {
        namespace observers {

            void BaseObserver::dispatchPacket(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    pico::ibus::data::IbusPacket iBusPacket) {

                logger->d(this->getTag(), fmt::format("Dispatching Packet"));
                this->onNewPacket(iBusPacket);
                logger->d(this->getTag(), fmt::format("Dispatched Packet"));
            }
        } // pico
    } // ibus
} // observers