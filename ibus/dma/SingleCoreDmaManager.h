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


namespace pico::ibus::dma {

        class SingleCoreDmaManager : public pico::ibus::dma::IDmaManager {

        private:
            std::shared_ptr<logger::BaseLogger> logger;
            //TODO initialize this variable
            inline static std::shared_ptr<logger::BaseLogger> staticLogger;
            std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry;

            inline static Packetizer fromPiQPacketizer;
            inline static Packetizer fromCarQPacketizer;

            inline static queue_t toPiQ;      //Packets
            inline static queue_t toCarQ;     //Packets

            inline static queue_t fromPiQ;    //Packets
            inline static queue_t fromCarQ;   //Packets

//            inline static std::array<uint8_t, 255> paddedUart0RxPacketBuffer;
//            inline static std::array<uint8_t, 255> paddedUart1RxPacketBuffer;

            void setupUarts();

            inline static void on_uart0_rx();
            inline static void on_uart1_rx();

            inline static void handleRxInterruptServiceRoutine(
                    uart_inst_t* uart,
                    std::string uartName,
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


            static void writePacketToQ(
                    std::shared_ptr<logger::BaseLogger> logger,
                    data::IbusPacket packet,
                    queue_t* toQ,
                    std::string toQName
                    );
        public:

            SingleCoreDmaManager(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry
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
