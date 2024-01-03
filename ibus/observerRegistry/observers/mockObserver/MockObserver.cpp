//
// Created by stefan on 12/2/22.
//

#include "MockObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            MockObserver::MockObserver(
                    std::shared_ptr<pico::logger::BaseLogger> logger,
                    std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager) {
                this->logger = logger;
                this->busTopologyManager = busTopologyManager;
                logger->d("MockObserver", "Constructed");
            }

            void MockObserver::onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) {
                if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
                    logger->d("MockObserver", iBusPacket->toString());
                }
            }
        } // pico
    } // ibus
} // observers