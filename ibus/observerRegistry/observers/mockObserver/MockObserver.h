//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_MOCKOBSERVER_H
#define PICOTEMPLATE_MOCKOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <logging/BaseLogger.h>
#include "ibus/dma/topology/BusTopologyManager.h"

namespace pico::ibus::observers {

            class MockObserver : public BaseObserver {

            public:
                MockObserver(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::ibus::topology::BusTopologyManager>
                        );

            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager;
            protected:
                std::string getTag() override { return "MockObserver"; };
                void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
            };

        } // observers

#endif //PICOTEMPLATE_MOCKOBSERVER_H
