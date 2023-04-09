//
// Created by stefan on 4/7/23.
//

#ifndef PICOTEMPLATE_DMAMANAGER_H
#define PICOTEMPLATE_DMAMANAGER_H

#include <memory>
#include "../../logging/BaseLogger.h"
#include "../../ibus/observerRegistry/ObserverRegistry.h"
#include "pico/util/queue.h"
#include <pico.h>
#include "../../ibus/data/IbusPacket.h"

namespace pico {
    namespace ibus {
        namespace dma {

            class DmaManager {

                //https://github.com/rossihwang/pico_dma_uart/blob/main/dma_uart.cpp

                //https://hackspace.raspberrypi.com/articles/what-is-programmable-i-o-on-raspberry-pi-pico
            private:

                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry;
                DmaManager(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry
                );

                //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#queue

                //A queue we use for CPU0 to offload
                //message prep and blocking to CPU1.
                queue_t outgoingQ; //Outgoing from pico.

                //Cpu1 will get interrupted often by
                //Incoming messages from IBUS. We allow
                //the CPU1 event loop to stall, and when
                //there's processing time, we peel off
                //the incomingQ onto CPU0, for all the
                //event handlers to run.
                queue_t incomingQ; //Incoming into pico.


                void setupFromCarPioProgram();
                void setupToCarPioProgram();

                void setupIncomingDmaInterruptServiceRoutine();

                void cpu1ScheduleOutgoingMessage();

                void shuffleOutgoingMessageToCpu1(data::IbusPacket packet); //Called from CPU0
                void shuffleIncomingMessageToCpu0(data::IbusPacket packet); //Called from CPU1.

                //Runs on Cpu1. Called when the interrupt service routine on cpu1 gets an Ibus packet.
                void onCpu1IncomingPacket(std::unique_ptr<data::IbusPacket> packet);

                //Runs on Cpu0. Called when cpu0 gets a packet out of the inter-processor queue and needs to
                //dispatch it to the Observer registry.
                void onCpu0IncomingPacket(std::unique_ptr<data::IbusPacket> packet);
            public:
                //Called only by ApplicationContainer
                void cpu0setup();
                void onCpu0Loop();
                void cpu1Setup();
                void onCpu1Loop();
                //Called only by ApplicationContainer

                void cpu0scheduleOutgoingMessage(data::IbusPacket packet); //Called from the base output writer.
            };

        } // pico
    } // ibus
} // dma

#endif //PICOTEMPLATE_DMAMANAGER_H
