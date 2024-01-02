//
// Created by stefan on 10/24/23.
//

#ifndef PICOTEMPLATE_SINGLECOREDMAMANAGER_H
#define PICOTEMPLATE_SINGLECOREDMAMANAGER_H

#include "IDmaManager.h"

#include <logging/BaseLogger.h>
#include <ibus/observerRegistry/ObserverRegistry.h>
#include "pico/util/queue.h"
#include "Packetizer.h"
#include "hardware/uart.h"
#include <pico.h>
#include <ibus/data/IbusPacket.h>

#include "fmt/format.h"

#include "PioPins.h"

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pico/sync.h"
#include "pico/scanvideo.h"
#include "ibus/dma/topology/BusTopologyManager.h"


namespace pico::ibus::dma {

        class SingleCoreDmaManager : public pico::ibus::dma::IDmaManager {

        private:
            std::shared_ptr<logger::BaseLogger> logger;
            //TODO initialize this variable
            inline static std::shared_ptr<logger::BaseLogger> staticLogger;
            std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry;

            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager;

            inline static Packetizer fromPiQPacketizer = Packetizer();
            inline static Packetizer fromCarQPacketizer = Packetizer();

            inline static queue_t toPiQ;      //Packets
            inline static queue_t toCarQ;     //Packets

            inline static queue_t fromPiQ;    //Packets
            inline static queue_t fromCarQ;   //Packets

//            inline static std::array<uint8_t, 255> paddedUart0RxPacketBuffer;
//            inline static std::array<uint8_t, 255> paddedUart1RxPacketBuffer;

            //It doesn't matter which IRQ/ISR is running,
            //we must protect a packetizer's state (it is non-reentrant)
            critical_section_t packetizerCs;


            void setupUarts();

            inline static void on_uart0_rx();
            inline static void on_uart1_rx();

            inline static void handleRxInterruptServiceRoutine(
                    uart_inst_t* uart,
                    queue_t* toQ
            );

            inline static queue_t uart0rxByteQ;
            inline static queue_t uart1rxByteQ;

            void flushUart0ByteBufferToPacketizer();
            void flushUart1ByteBufferToPacketizer();
            void flushUartByteBufferToPacketizer(
                    queue_t* fromByteQ,
                    std::string fromByteQName,
                    queue_t* toQ,
                    std::string toQName,
                    Packetizer* packetizer
                    );

            void flushFromPiQToLogic();
            void flushFromCarQToLogic();
            void readIncomingQ_toLogic(
                queue_t* queue,
                std::string queue_name
            );

            void flushToPiQToUart(); //Flush Pi Q to Uart
            void flushToCarQToUart(); //Flush ToCarQ to Uart
            inline static std::array<uint8_t, 255> incomingPacketBuffer;
            void writeOutgoingQ_toUart(
                    queue_t* movePacketFrom,
                    uart_inst_t* uart,
                    std::string fromName,
                    std::string uartName
            );

            static const bool statusWriteEnabled = true;
            uint32_t lastWrittenLoopIteration = 0;
            uint32_t currentLoopIteration = 0;
            uint16_t writeStatusEvery = 300;

            bool shouldWriteStatus(); //Could be some sort of "on x iterations" logic.
            static void writeStatus(std::shared_ptr<logger::BaseLogger> logger);


            void writePacketToQ(
                    data::IbusPacket packet,
                    queue_t* toQ,
                    std::string toQName
                    );
        public:

            SingleCoreDmaManager(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry,
                    std::shared_ptr<pico::ibus::topology::BusTopologyManager>
            );

            void cpu0setup() override;
            void onCpu0Loop() override;
            void cpu1Setup() override;
            void onCpu1Loop() override;

            void cpu0scheduleOutgoingMessage(data::IbusPacket packet) override; //Called from the base output writer.

            void cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) override;

            ~SingleCoreDmaManager() override = default;

        };

        } // dma

#endif //PICOTEMPLATE_SINGLECOREDMAMANAGER_H
