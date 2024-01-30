//
// Created by stefan on 10/24/23.
//

#include "SingleCoreDmaManager.h"

namespace pico::ibus::dma {

    SingleCoreDmaManager::SingleCoreDmaManager(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry,
            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager
            ) {
        this->logger = logger;
        this->observerRegistry = observerRegistry;

        if (this->observerRegistry == nullptr) {
            logger->wtf("SingleCoreDmaManager", "observerRegistry is null");
        }

        this->busTopologyManager = busTopologyManager;

        //TODO length==255 might actually be too short. Because, the packet length could be 255+1?

        queue_init(
                &uart0rxByteQ,
                sizeof(uint8_t),
                2048
                );

        queue_init(
                &uart1rxByteQ,
                sizeof(uint8_t),
                2048
                );

        queue_init(
                &toPiQ,
                255,
                10
                );

        queue_init(
                &toCarQ,
                255,
                10
                );

        queue_init(
                &fromCarQ,
                255,
                10
                );
        queue_init(
                &fromPiQ,
                255,
                10
                );

        staticLogger = logger;

        critical_section_init(&packetizerCs);
    }

    void SingleCoreDmaManager::cpu0setup() {
        setupUarts();
    }

    void SingleCoreDmaManager::onCpu0Loop() {
        messagePumpRunning = true;
        scanvideo_wait_for_vblank();
        flushUart0ByteBufferToPacketizer();
        flushUart1ByteBufferToPacketizer();
        flushFromPiQToLogic();
        flushFromCarQToLogic();
        //TODO build a pass-through observer that takes all uart1 RX packets and sends them to uart0

        flushToPiQToUart();
        flushToCarQToUart();

//        if (shouldWriteStatus()) {
//            writeStatus(logger);
//        }
    }

    void SingleCoreDmaManager::cpu1Setup() { /* NOOP */ }

    void SingleCoreDmaManager::onCpu1Loop() { /* NOOP */}


    void SingleCoreDmaManager::setupUarts() {

        //Give us the ability to turn off the upstream source from "the car" while we're setting up uarts.
        gpio_set_function(LIN_ChipSelect, GPIO_FUNC_SIO);
        gpio_set_dir(LIN_ChipSelect, true);


        //TODO STEFAN I don't think we reliably enter the TX Enable state with this setup code.

        gpio_set_function(LIN_Fault, GPIO_FUNC_SIO); //when low, TX to car is off.
        gpio_set_dir(LIN_Fault, true); //TXe/LFAULT is an output pin.

        gpio_put(LIN_Fault, true);//TX to car is on.
        gpio_put(LIN_ChipSelect, true); //Turn off TX to car while we set up.

        //Set the pin functions before we do stuff with the UART.
        gpio_set_function(UART0_LIN_TRANS_RX, GPIO_FUNC_UART);
        gpio_set_function(UART0_LIN_TRANS_TX, GPIO_FUNC_UART);

        gpio_set_function(UART1_PICOPROBE_RX, GPIO_FUNC_UART);
        gpio_set_function(UART1_PICOPROBE_TX, GPIO_FUNC_UART);


        //Uart0 always goes to MCP2020 to IBUS
        //We're 9600 8E1, just like the modbmw interface
        uart_init(uart0, 9600);
        uart_set_hw_flow(uart0, false, false);
        uart_set_format(uart0, 8, 1, UART_PARITY_EVEN); //8E1

        switch (busTopologyManager->getBusToplogy()) {
            case topology::BusTopology::CAR_WITH_PI:
                //We're 9600 8E1, just like the modbmw interface
                uart_init(uart1, 9600);
                uart_set_hw_flow(uart1, false, false);
                uart_set_format(uart1, 8, 1, UART_PARITY_EVEN); //8E1
                break;
            case topology::BusTopology::SLED_NO_PI:
            case topology::BusTopology::SLED_LAPTOP_HMI:
                uart_init(uart1, 115200);
                uart_set_hw_flow(uart1, false, false);
                uart_set_format(uart1, 8, 1, UART_PARITY_NONE); //8N1
                break;
        }

        //TODO do we want an IRQ when the fault line on the lin transceiver goes high?
        //TODO what should we do then? Maybe add a method to nuke everything and re-setup DMAManager?

        //Keep the fifos on because we're going to use Q's to buffer the bytes out of the ISR.
        uart_set_fifo_enabled(uart0, true);
        uart_set_fifo_enabled(uart1, true);

        irq_set_exclusive_handler(UART0_IRQ, on_uart0_rx);
        irq_set_enabled(UART0_IRQ, true);
        uart_set_irq_enables(uart0, true, false);

        irq_set_exclusive_handler(UART1_IRQ, on_uart1_rx);
        irq_set_enabled(UART1_IRQ, true);
        uart_set_irq_enables(uart1, true, false);


        //gpio_put(LIN_Fault, false); //TX to car enabled.

        //Now that we're done the setup, turn on the lin transeiver
        //gpio_put(LIN_ChipSelect, true);


        if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI ||
            busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_LAPTOP_HMI) {
            //This will cause printf's from the StdioPrintFlogger to also show up
            //on the UART, as well as writes from this class.
            stdio_uart_init_full(uart1, 115200, UART1_PICOPROBE_TX, UART1_PICOPROBE_RX);
        }

        if (busTopologyManager->getBusToplogy() == topology::BusTopology::CAR_WITH_PI) {
            //CMake flags have caused only the USB stdio to be enabled, but we still
            //have to enable stdio on the usb port for debugging.
            stdio_init_all();
        }

        //TODO now what we need to do is make sure all our write outs to UART1 are actually going
        //TODO to the usb stdout.

        //Not sure if this helps.
        //sleep_ms(200);

        //fromCarQPacketizer.writeState("SingleCoreDmaManagerConstructor_fromCarQ", logger);
        //fromPiQPacketizer.writeState("SingleCoreDmaManagerConstructor_fromPiQ", logger);

        //stdio_init_all();
    }

    void SingleCoreDmaManager::on_uart0_rx() {
        handleRxInterruptServiceRoutine(uart0, &uart0rxByteQ);
    }

    void SingleCoreDmaManager::on_uart1_rx() {
        //handleRxInterruptServiceRoutine(uart1, &uart1rxByteQ);
    }

    void SingleCoreDmaManager::flushUart0ByteBufferToPacketizer() {
        flushUartByteBufferToPacketizer(
                &uart0rxByteQ,
                "uart0rxByteQ",
                &fromCarQ,
                "fromCarQ",
                &fromCarQPacketizer
                );
    }
    void SingleCoreDmaManager::flushUart1ByteBufferToPacketizer() {
        flushUartByteBufferToPacketizer(
                &uart1rxByteQ,
                "uart1rxByteQ",
                &fromPiQ,
                "fromPiQ",
                &fromPiQPacketizer
        );
    }

    void SingleCoreDmaManager::flushUartByteBufferToPacketizer(
            queue_t *fromByteQ,
            std::string fromByteQName,
            queue_t *toQ,
            std::string toQName,
            Packetizer *packetizer
    ) {

        uint8_t byte;

        //We're going to depend on the application container loop here so that we don't block here a bunch.
        //We will add one byte at a time to the packetizer.

        bool haveByte = queue_try_remove(fromByteQ, &byte);
        if (haveByte) {
            critical_section_enter_blocking(&packetizerCs);
                packetizer->addByte(byte);
                if (packetizer->isPacketComplete()) {
                    //fromCarQPacketizer.writeState("SingleCoreDmaManagerISR_fromCarQ", logger);
                    writePacketToQ(
                            data::IbusPacket(packetizer->getPacketBytes()),
                            toQ,
                            toQName
                    );
                    packetizer->reset();
                }
            critical_section_exit(&packetizerCs);
        }
    }


    void SingleCoreDmaManager::handleRxInterruptServiceRoutine(
            uart_inst_t *uart,
            queue_t *toQ
    ) {
        //https://github.com/raspberrypi/pico-examples/blob/master/uart/uart_advanced/uart_advanced.c
        while (uart_is_readable(uart)) {
            uint8_t ch = uart_getc(uart);
            //uart_getc(uart) in the standard library casts to `char`, which might be signed.
//            uart_read_blocking(uart, &ch, 1);
            if (!queue_try_add(toQ, &ch)) {
                //We couldn't write the queue?
                queue_free(toQ);
            }
        }
    }

    void SingleCoreDmaManager::writeOutgoingQ_toUart(
            queue_t *movePacketFrom,
            uart_inst_t *uart,
            std::string fromName,
            std::string uartName) {

        bool log_outgoingPacket = false;

        //take the 255-byte buffer and parse it into a packet
        //then, if it's valid,
        //then do a uart send_blocking for len == packet.rawBytes().length
        //that way we don't clog the bus sending 0's.

        std::array<uint8_t, 255> outgoingPacketBuffer = std::array<uint8_t , 255>();
        outgoingPacketBuffer.fill(0);

        if (queue_try_remove(movePacketFrom, outgoingPacketBuffer.data())) {


            if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
                //Guard to prevent infinite logging loop on topologies that output log messages over ibus.
                logger->d("DmaManager",
                          fmt::format("We have a packet from {} to write to uart {}", fromName, uartName));
            }

            auto* packet = new data::IbusPacket(outgoingPacketBuffer);
            if (packet->isPacketValid()) {
                //TODO is is right to write out getRawPacket().size? What if it's really big?
                //TODO what if we accidentally skip the checksum byte at the end?
                if (log_outgoingPacket) {
                    logger->d("SingleCoreDmaManager_outgoing", packet->toString());
                    logger->d("SingleCoreDmaManager_outgoing", fmt::format("RawPacket Size: {}", packet->getRawPacket().size()));
                }


                auto rd = packet->getRawPacket().data();
                auto rs = packet->getRawPacket().size();

//                uart_write_blocking(uart, packet->getRawPacket().data(), packet->getRawPacket().size());
                uart_write_blocking(uart, rd, rs);

                if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
                    //Guard to prevent infinite logging loop on topologies that output log messages over ibus.
                    logger->d("DmaManager",
                              fmt::format("Wrote packet from {} to write to uart {}", fromName, uartName));
                }
            } else {
                logger->wtf("DmaManager", "Trying to write out an invalid packet??");
                logger->wtf("DmaManager", fmt::format("Invalid Packet is {}", packet->toString()));
            }
        }
    }


    void SingleCoreDmaManager::flushFromCarQToLogic() {
        readIncomingQ_toLogic(&fromCarQ, "fromCarQ");
    }

    void SingleCoreDmaManager::flushFromPiQToLogic() {
        readIncomingQ_toLogic(&fromPiQ, "fromPiQ");
    }

    void SingleCoreDmaManager::readIncomingQ_toLogic(queue_t *queue, std::string queue_name) {

        bool log_dispatch_trace = false;

        std::array<uint8_t, 255> buffer = std::array<uint8_t , 255>();
        buffer.fill(0); //Fill the buffer with zeros so the .data() below dereferences validly.

        bool havePacket = queue_try_remove(queue, (void*) buffer.data());
        if (havePacket) {
            //A packet came into the pico for processing.
            if (log_dispatch_trace) {
                logger->d("SingleCoreDmaManager",
                          fmt::format("Dispatching packet from Q {} to cpu0 observers", queue_name));
            }

            data::IbusPacket packetFromArray = data::IbusPacket(buffer);

            if (queue == &fromCarQ) {
                packetFromArray.setPacketSource(data::PacketSource::FROM_CAR);
            }

            if (queue == &fromPiQ) {
                packetFromArray.setPacketSource(data::PacketSource::FROM_PI);
            }

            observerRegistry->dispatchMessageToAllObservers(packetFromArray);

            if (log_dispatch_trace) {
                logger->d("SingleCoreDmaManager",
                          fmt::format("Finished dispatching packet from Q {} to cpu0 observers", queue_name));
            }
        }

    }


    void SingleCoreDmaManager::flushToCarQToUart() {
        writeOutgoingQ_toUart(
                &toCarQ,
                uart0,
                "ToCarQ",
                "uart0"
        );
    }

    void SingleCoreDmaManager::flushToPiQToUart() {
        writeOutgoingQ_toUart(
                &toPiQ,
                uart1,
                "ToPiQ",
                "uart1"
        );
    }


    bool SingleCoreDmaManager::shouldWriteStatus() {
        if (!statusWriteEnabled) { return false; }

        currentLoopIteration = currentLoopIteration + 1;

        if (currentLoopIteration >= lastWrittenLoopIteration + writeStatusEvery) {
            lastWrittenLoopIteration = currentLoopIteration;
            return true;
        }

        return false;
    }

    void SingleCoreDmaManager::writeStatus(
            std::shared_ptr<logger::BaseLogger> logger
            ) {

        uint toPiQ_level = queue_get_level(&toPiQ);
        uint toCarQ_level = queue_get_level(&toCarQ);
        uint fromPiQ_level = queue_get_level(&fromPiQ);
        uint fromCarQ_level = queue_get_level(&fromCarQ);

        uint uart0rxByteQ_level = queue_get_level(&uart0rxByteQ);
        uint uart1rxByteQ_level = queue_get_level(&uart1rxByteQ);

        // If true, don't log out when we have bytes.
        bool ignoreByteQs = true;

        if (toPiQ_level == 0 &&
            toCarQ_level == 0 &&
            fromPiQ_level == 0 &&
            fromCarQ_level == 0 &&
                (ignoreByteQs || (uart0rxByteQ_level == 0 && uart1rxByteQ_level == 0))
        ) {
            //Don't print zeroes since it's slow to do stdout with no news.
            return;
        }

        //fromCarQPacketizer.writeState("SingleCoreDmaManagerWriteStatus_fromCarQ", logger);
        //fromPiQPacketizer.writeState("SingleCoreDmaManagerWriteStatus_fromPiQ", logger);

        logger->d(
                "SingleCoreDmaManager",
                fmt::format(
                        "Cpu0 Queue Health: "
                        "toPiQ: {:d}, "
                        "toCarQ: {:d}, "
                        "fromPiQ: {:d}, "
                        "fromCarQ: {:d}, "
                        "uart0rxByteQ: {:d}, "
                        "uart1rxByteQ: {:d} ",
                        toPiQ_level,
                        toCarQ_level,
                        fromPiQ_level,
                        fromCarQ_level,
                        uart0rxByteQ_level,
                        uart1rxByteQ_level
                ));
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) {
        writePacketToQ(
                packet,
                &toPiQ,
                "toPiQ"
        );
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingMessage(data::IbusPacket packet) {
        writePacketToQ(
                packet,
                &toCarQ,
                "toCarQ"
        );
    }

    void SingleCoreDmaManager::writePacketToQ(
            data::IbusPacket packet,
            queue_t *toQ,
            std::string toQName
    ) {

        if (!messagePumpRunning) {
            //We're not emptying the queues yet, so no point wasting time storing
            //chatty messages that will get deleted. Our queue depths are memory-constrained,
            //and we can't get the queues emptying before they're filled up.
            return;
        }

        bool log_writePacketToQ = false;

        if (log_writePacketToQ) {
            staticLogger->d("SingleCoreDmaManager", fmt::format("Writing packet to q {}", toQName));
        }

        std::vector<uint8_t> rawPacket = packet.getRawPacket();

        std::array<uint8_t, 255> outgoingPacketBuffer = std::array<uint8_t , 255>();
        outgoingPacketBuffer.fill(0);

        uint8_t index = 0;
        for (uint8_t byte : rawPacket) {
            outgoingPacketBuffer[index++] = byte;
        }

        bool wasAdded = queue_try_add(toQ, outgoingPacketBuffer.data());

        if (!wasAdded) {
            //What happens on startup is that all the constructors call lots of log messages,
            //and nothing clears out the toPiQ until all the constructors have run.
            //So, what we'll do is just empty the queue.
            //TODO we need a flag in this class that is set when onCpu0Loop runs for the first time.
            //TODO don't add messages to the outgoing message queue until we have a message pump.
            while(!queue_is_empty(toQ)){
                std::array<uint8_t, 255> temp = std::array<uint8_t , 255>();
                queue_try_remove(toQ, temp.data());
            }
            staticLogger->d("SingleCoreDmaManager", fmt::format("Tried emptying Q {}", toQName));
            writeStatus(logger);
        }
    }


} // dma