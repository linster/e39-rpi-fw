//
// Created by stefan on 4/7/23.
//

#ifndef PICOTEMPLATE_DMAMANAGER_H
#define PICOTEMPLATE_DMAMANAGER_H

#include <cstring>
#include <memory>
#include "../../logging/BaseLogger.h"
#include "../../ibus/observerRegistry/ObserverRegistry.h"
#include "pico/util/queue.h"
#include <pico.h>
#include "../../ibus/data/IbusPacket.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"

#include "FromCarProgram.pio.h"
#include "ToCarProgram.pio.h"

#include "PioPins.h"
#include "Packetizer.h"

#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace dma {

            class DmaManager {

                //This class is mis-named because we're not actually doing DMA. But someday, I hope to.
                //https://github.com/rossihwang/pico_dma_uart/blob/main/dma_uart.cpp

            private:

                static std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry;

                //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#queue

                //A queue we use for CPU0 to offload
                //message prep and blocking to CPU1.
                static queue_t outgoingQ; //Packets from Pico

                //Cpu1 will get interrupted often by
                //Incoming messages from IBUS. We allow
                //the CPU1 event loop to stall, and when
                //there's processing time, we peel off
                //the incomingQ onto CPU0, for all the
                //event handlers to run.
                static queue_t incomingQ;   //Packets into Pico


                static queue_t fromProbeQ; //Raw bytes
                static Packetizer fromProbeQPacketizer;
                static queue_t fromCarQ;   //Raw Bytes
                static Packetizer fromCarQPacketizer;

                static queue_t toProbeQ;    //Packets
                static queue_t toCarQ;      //Packets



                inline static void on_uart0_rx();
                inline static void on_uart1_rx();

                inline static void handleRxInterruptServiceRoutine(
                        uart_inst_t* uart,
                        std::string uartName,
                        queue_t* toQ,
                        std::string toQName,
                        Packetizer* packetizer
                );

                void flushFromCarQ();
                void flushFromProbeQ();
                void flushOutgoingQ();

                void fanoutPacketsFromQ_to2Qs_nonBlocking(
                        queue_t* moveFrom,
                        queue_t* to0,
                        queue_t* to1,
                        std::string fromName,
                        std::string to0Name,
                        std::string to1Name
                );

                void flushToProbeQ();
                void flushToCarQ();
                void writeOutgoingQ_toUart(
                        queue_t* movePacketFrom,
                        uart_inst_t* uart,
                        std::string fromName,
                        std::string uartName
                        );

                //Runs on Cpu0. Called when cpu0 gets a packet out of the inter-processor queue and needs to
                //dispatch it to the Observer registry.
                void onCpu0IncomingPacket(std::unique_ptr<data::IbusPacket> packet);

            public:

                DmaManager(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry
                );

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
