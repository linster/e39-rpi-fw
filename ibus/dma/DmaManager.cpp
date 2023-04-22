//
// Created by stefan on 4/7/23.
//

#include "DmaManager.h"

namespace pico {
    namespace ibus {
        namespace dma {

            DmaManager::DmaManager(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry
                    ) {
                this->logger = logger;
                staticLogger = logger;
                this->observerRegistry = observerRegistry;

                queue_init(
                        &(pico::ibus::dma::DmaManager::incomingQ),
                        255, //Assume each packet is 255 bytes long. (Max_length for the rawBytes)
                        8
                );

                queue_init(
                        &outgoingQ,
                        255,
                        8
                );

                queue_init(
                        &outgoingProbeOnlyQ,
                        255,
                        8
                );

                queue_init(
                        &fromCarQ,
                        255,
                        8
                );

                queue_init(
                        &fromProbeQ,
                        255,
                        8
                        );

                queue_init(
                        &toCarQ,
                        255,
                        8
                        );
                queue_init(
                        &toProbeQ,
                        255,
                        8
                        );

                fromCarQPacketizer = Packetizer();
                fromProbeQPacketizer = Packetizer();
            }


            //LIN Transeiver
            void DmaManager::on_uart0_rx() {
                handleRxInterruptServiceRoutine(
                        uart0,
                        "uart0",
                        &fromCarQ,
                        "fromCarQ",
                        &fromCarQPacketizer
                );
            }

            //PicoProbe/Application Processor
            void DmaManager::on_uart1_rx() {
                handleRxInterruptServiceRoutine(
                        uart1,
                        "uart1",
                        &fromProbeQ,
                        "fromProbeQ",
                        &fromProbeQPacketizer
                );
            }

            void DmaManager::handleRxInterruptServiceRoutine(
                    uart_inst_t *uart,
                    std::string uartName,
                    queue_t *toQ,
                    std::string toQName,
                    Packetizer* packetizer
            ) {
                //https://github.com/raspberrypi/pico-examples/blob/master/uart/uart_advanced/uart_advanced.c
                while (uart_is_readable(uart)) { //TODO look up how to do inline assignment and comparison?
                    packetizer->writeState(fmt::format("DmaManager Rx ISR Uart: {}", uartName), staticLogger);
                    char newByte = uart_getc(uart);
                    packetizer->addByte(newByte);
                    packetizer->writeState(fmt::format("DmaManager Rx ISR Uart: {}", uartName), staticLogger);
                    if (packetizer->isPacketComplete()) {
                        //Shuffle the packet out.
                        staticLogger->d("DmaManager", fmt::format("Got complete packet from uart {}", uartName));

                        void *paddedPacketBuffer = std::calloc(255, 1); //TODO no memory allocation allowed in interrupt service routes lol.
                        std::memcpy(
                                paddedPacketBuffer,
                                (void *) packetizer->getPacketBytes().data(),
                                packetizer->getPacketBytes().size()
                        );
                        staticLogger->d("DmaManager", fmt::format("(uart: {}) Sending Padded buffer to Q: {}", uartName, toQName));
                        queue_add_blocking(toQ, paddedPacketBuffer);
                        free(paddedPacketBuffer);

                        packetizer->recycle(); //Shuffle what's left in the packetizer to the front.

                        if (packetizer->isPacketComplete()) {
                            //Check just in case we have a complete packet so we're not waiting on one more byte to come in
                            //Before sending out a packet.
                            staticLogger->d("DmaManager", fmt::format("After recycle for packetizer for uart {}, we have a complete packet.", uartName));
                            void *paddedPacketBufferAfterRecycle = std::calloc(255, 1);
                            std::memcpy(
                                    paddedPacketBufferAfterRecycle,
                                    (void *) packetizer->getPacketBytes().data(),
                                    packetizer->getPacketBytes().size()
                            );
                            staticLogger->d("DmaManager", fmt::format("(uart: {}) Sending Padded buffer to Q: {}", uartName, toQName));
                            queue_add_blocking(toQ, paddedPacketBufferAfterRecycle);
                            free(paddedPacketBufferAfterRecycle);
                            packetizer->recycle();
                        }
                    }
                }
            }

            void DmaManager::cpu0setup() {

                logger->d("DmaManager", "cpu0Setup");

                uart_init(uart0, 9600);
                uart_init(uart1, 9600);

                uart_set_hw_flow(uart0, false, false);
                uart_set_hw_flow(uart1, false, false);

                uart_set_format(uart0, 8, 1, UART_PARITY_EVEN); //8E1
                uart_set_format(uart1, 8, 1, UART_PARITY_EVEN); //8E1

                gpio_set_function(UART0_LIN_TRANS_RX, GPIO_FUNC_UART);
                gpio_set_function(UART0_LIN_TRANS_TX, GPIO_FUNC_UART);

                gpio_set_function(UART1_PICOPROBE_RX, GPIO_FUNC_UART);
                gpio_set_function(UART1_PICOPROBE_TX, GPIO_FUNC_UART);

                gpio_set_dir(LIN_ChipSelect, true);
                gpio_pull_down(LIN_ChipSelect); //Should already be pulled down in the chip.

                //TODO do we want an IRQ when the fault line on the lin transceiver goes high?
                //TODO what should we do then? Maybe add a method to nuke everything and re-setup DMAManager?

                logger->d("DmaManager", "cpu0Setup done.");
            }

            void DmaManager::cpu1Setup() {
                //LIN Transceiver RX. Need to shuffle those bytes to PICOPROBE TX, and also collec them
                //in a buffer locally.
                logger->d("DmaManager", "cpu1Setup");
                irq_add_shared_handler(
                        UART0_IRQ,
                        on_uart0_rx,
                        PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY
                        );
                irq_set_enabled(UART0_IRQ, true);
                uart_set_irq_enables(uart0, true, false);

                irq_add_shared_handler(
                        UART1_IRQ,
                        on_uart1_rx,
                        PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY
                        );
                irq_set_enabled(UART1_IRQ, true);
                uart_set_irq_enables(uart1, true, false);


                //Now that we're done the setup, turn on the lin transeiver
                gpio_put(LIN_ChipSelect, true);

                logger->d("DmaManager", "cpu1 Setup done.");
            }

            void DmaManager::onCpu0Loop() {
                //We check the incoming queue without blocking on empty. If there's something in there,
                //We dispatch it to the observer registry.

                logger->d("DmaManager", "onCpu0Loop");

                uint8_t* incomingPacketBuffer[255] = {0};
                bool havePacket = queue_try_remove(&incomingQ, (void*)incomingPacketBuffer);
                if (havePacket) {
                    logger->d("DmaManager" ,"onCpu0Loop have packet");
                    //Heap-allocate a new packet from the buffer.
                    std::unique_ptr<data::IbusPacket> newPacket = std::make_unique<data::IbusPacket>(new data::IbusPacket(incomingPacketBuffer));
                    onCpu0IncomingPacket(std::move(newPacket));
                }
            }

            void DmaManager::onCpu1Loop() {
                logger->d("DmaManager", "onCpu1Loop");
                flushOutgoingQ();
                flushOutgoingProbeOnlyQ();
                flushFromCarQ();
                flushFromProbeQ();

                flushToProbeQ();
                flushToCarQ();
                logger->d("DmaManager", "onCpu1Loop Done");
            }

            void DmaManager::flushFromProbeQ() {
                fanoutPacketsFromQ_to2Qs_nonBlocking(
                        &fromProbeQ,
                        &toCarQ,
                        &incomingQ,
                        "fromProbeQ",
                        "toCarQ",
                        "incomingQ"
                );
            }

            void DmaManager::flushFromCarQ() {
                fanoutPacketsFromQ_to2Qs_nonBlocking(
                        &fromCarQ,
                        &toProbeQ,
                        &incomingQ,
                        "fromCarQ",
                        "toProbeQ",
                        "incomingQ"
                );
            }

            void DmaManager::flushOutgoingQ() {
                fanoutPacketsFromQ_to2Qs_nonBlocking(
                        &outgoingQ,
                        &toProbeQ,
                        &toCarQ,
                        "outGoingQ",
                        "toProbeQ",
                        "toCarQ"
                        );
            };

            //TODO someday explore templates, or passing in a vector of stuff.
            void DmaManager::fanoutPacketsFromQ_to2Qs_nonBlocking(
                    queue_t* moveFrom,
                    queue_t* to0,
                    queue_t* to1,
                    std::string fromName,
                    std::string to0Name,
                    std::string to1Name
                    ) {

                assert(moveFrom->element_size == 255);
                assert(to0->element_size == 255);
                assert(to1->element_size == 255);

                void* fromPacket = std::calloc(255, 1);

                bool packetRemoved = queue_try_remove(moveFrom, &fromPacket);
                if (packetRemoved) {
                    logger->d("DmaManager", fmt::format("Removed packet from {}. About to move to {} and {}", fromName, to0Name, to1Name));

                    //Copy the packet to0
                    queue_add_blocking(to0, fromPacket);
                    logger->d("DmaManager", fmt::format("Copied packet from {} into {}. Remain to copy into {}", fromName, to0Name, to1Name));
                    //Copy the packet to1
                    queue_add_blocking(to1, fromPacket);
                    logger->d("DmaManager", fmt::format("Copied packet from {} into {} and {}. Done transfer.", fromName, to0Name, to1Name));
                } else {
//                    logger->d("DmaManager", fmt::format("No packet to remove from queue {}", fromName));
                }

                free(fromPacket);
            }

            void DmaManager::flushOutgoingProbeOnlyQ() {
                fanoutPacketsFromQ_toQ_nonBlocking(
                        &outgoingProbeOnlyQ,
                        &toProbeQ,
                        "OutgoingProbeOnlyQ",
                        "ToProbeQ"
                );
            }

            void DmaManager::fanoutPacketsFromQ_toQ_nonBlocking(
                    queue_t *moveFrom,
                    queue_t *to0,
                    std::string fromName,
                    std::string to0Name
            ) {
                assert(moveFrom->element_size == 255);
                assert(to0->element_size == 255);

                void* fromPacket = std::calloc(255, 1);

                bool packetRemoved = queue_try_remove(moveFrom, &fromPacket);
                if (packetRemoved) {
                    logger->d("DmaManager", fmt::format("Removed packet from {}. About to move to {}", fromName, to0Name));
                    //Copy the packet to0
                    queue_add_blocking(to0, fromPacket);
                    logger->d("DmaManager", fmt::format("Copied packet from {} into {}.", fromName, to0Name));

                } else {
//                    logger->d("DmaManager", fmt::format("No packet to remove from queue {}", fromName));
                }

                std::free(fromPacket);
            }

            void DmaManager::flushToCarQ() {
                writeOutgoingQ_toUart(
                        &toCarQ,
                        uart0,
                        "toCarQ",
                        "uart0 (LIN Transceiver)"
                        );
            }

            void DmaManager::flushToProbeQ() {
                writeOutgoingQ_toUart(
                        &toProbeQ,
                        uart1,
                        "toProbeQ",
                        "uart1 (PicoProbe / Rpi)"
                );
            }

            void DmaManager::writeOutgoingQ_toUart(
                    queue_t *movePacketFrom,
                    uart_inst_t *uart,
                    std::string fromName,
                    std::string uartName
            ) {
                //take the 255-byte buffer and parse it into a packet
                //then, if it's valid,
                //then do a uart send_blocking for len == packet.rawBytes().length
                //that way we don't clog the bus sending 0's.

                void* packetBuffer = std::calloc(255, 1);

                if (queue_try_remove(movePacketFrom, packetBuffer)) {
                    logger->d("DmaManager", fmt::format("We have a packet from {} to write to uart {}", fromName, uartName));
                    auto* packet = new data::IbusPacket(packetBuffer);
                    if (packet->isPacketValid()) {
                        uart_write_blocking(uart, packet->getRawPacket().data(), packet->getRawPacket().size());
                        logger->d("DmaManager",
                                  fmt::format("Wrote packet from {} to write to uart {}", fromName, uartName));
                    } else {
                        logger->wtf("DmaManager", "Trying to write out an invalid packet??");
                        logger->wtf("DmaManager", fmt::format("Invalid Packet is {}", packet->toString()));
                    }
                }
                free(packetBuffer);
            }

            //TODO figure out how to ignore the static warning. We want the caller of this method to have a reference
            //to DMAManager because that means the constructor will have run.
            void DmaManager::cpu0scheduleOutgoingMessage(data::IbusPacket packet) {
                //Do a blocking write into the outgoing queue.
                //The outgoing queue will feed the ToCar and FromCar programs.
                //There won't be a duplicated event, because the interrupt service routine

                //we need to pad the pointers returned by data() so that we're always
                //inserting 255-byte blocks, because queue_add_blocking will always
                //use the underlying Q's [size] parameter.
                //otherwise, we'll overrun the pointer
                void* paddedPacketBuffer = std::calloc(255, 1);
                std::memcpy(paddedPacketBuffer, (void*) packet.getRawPacket().data(), packet.getRawPacket().size());
                queue_add_blocking(&outgoingQ, paddedPacketBuffer); //Q copies the buffer, so we can free it now.
                free(paddedPacketBuffer);
            }


            void DmaManager::cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) {
                void* paddedPacketBuffer = std::calloc(255, 1);
                std::memcpy(paddedPacketBuffer, (void*) packet.getRawPacket().data(), packet.getRawPacket().size());
                queue_add_blocking(&outgoingProbeOnlyQ, paddedPacketBuffer); //Q copies the buffer, so we can free it now.
                free(paddedPacketBuffer);
            }

            void DmaManager::onCpu0IncomingPacket(std::unique_ptr<data::IbusPacket> packet) {
                //A packet came into the pico for processing.
                observerRegistry->dispatchMessageToAllObservers(*packet);
            }


        } // pico
    } // ibus
} // dma