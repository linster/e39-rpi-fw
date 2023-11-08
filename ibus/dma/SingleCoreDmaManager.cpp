//
// Created by stefan on 10/24/23.
//

#include "SingleCoreDmaManager.h"

namespace pico::ibus::dma {

    SingleCoreDmaManager::SingleCoreDmaManager(
            std::shared_ptr<logger::BaseLogger> logger,
            std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry) {
        this->logger = logger;
        this->observerRegistry = observerRegistry;

        //TODO length==255 might actually be too short. Because, the packet length could be 255+1?

        queue_init(
                &uart0rxByteQ,
                1,
                576
                );

        queue_init(
                &uart1rxByteQ,
                1,
                576
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
    }

    void SingleCoreDmaManager::cpu0setup() {
        setupUarts();
    }

    void SingleCoreDmaManager::onCpu0Loop() {
        flushUart0ByteBufferToPacketizer();
        flushUart1ByteBufferToPacketizer();
        flushFromPiQToLogic();
        flushFromCarQToLogic();

        flushToPiQToUart();
        flushToCarQToUart();

        if (shouldWriteStatus()) {
            writeStatus(logger);
        }
    }

    void SingleCoreDmaManager::cpu1Setup() { /* NOOP */ }

    void SingleCoreDmaManager::onCpu1Loop() { /* NOOP */}


    void SingleCoreDmaManager::setupUarts() {

        //We're 9600 8E1, just like the modbmw interface
        uart_init(uart0, 9600);
        uart_set_hw_flow(uart0, false, false);
        uart_set_format(uart0, 8, 1, UART_PARITY_EVEN); //8E1

//        //Always use the Adafruit 954 to go from Pico -> PC.
//        uart_init(uart1, 9600);
//        uart_set_hw_flow(uart1, false, false);
//        uart_set_format(uart1, 8, 1, UART_PARITY_EVEN); //8E1

        bool hookedUpToPicoProbe = true; //Set to false if the UART 1 is sent to rpi rx/tx pins
        if (hookedUpToPicoProbe) {
            uart_init(uart1, 115200);
            uart_set_hw_flow(uart1, false, false);
            uart_set_format(uart1, 8, 1, UART_PARITY_NONE); //8N1
        } else {
            uart_init(uart1, 9600);
            uart_set_hw_flow(uart1, false, false);
            //We're 9600 8E1, just like the modbmw interface
            uart_set_format(uart1, 8, 1, UART_PARITY_EVEN); //8E1
        }

        gpio_set_function(UART0_LIN_TRANS_RX, GPIO_FUNC_UART);
        gpio_set_function(UART0_LIN_TRANS_TX, GPIO_FUNC_UART);

        gpio_set_function(UART1_PICOPROBE_RX, GPIO_FUNC_UART);
        gpio_set_function(UART1_PICOPROBE_TX, GPIO_FUNC_UART);

        gpio_set_function(LIN_ChipSelect, GPIO_FUNC_SIO);
        gpio_set_dir(LIN_ChipSelect, true);
        //gpio_pull_down(LIN_ChipSelect); //Should already be pulled down in the chip.

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

        gpio_set_function(LIN_ChipSelect, GPIO_FUNC_SIO);
        gpio_set_dir(LIN_Fault, true);
        gpio_put(LIN_Fault, false); //Set TX enable

        //Now that we're done the setup, turn on the lin transeiver
        gpio_put(LIN_ChipSelect, true);
    }

    void SingleCoreDmaManager::on_uart0_rx() {
        handleRxInterruptServiceRoutine(uart0, &fromCarQ);
    }

    void SingleCoreDmaManager::on_uart1_rx() {
        handleRxInterruptServiceRoutine(uart1, &fromPiQ);
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
        bool haveByte = queue_try_remove(fromByteQ, &byte);
        if (haveByte) {
            packetizer->addByte(byte);
            if (packetizer->isPacketComplete()) {
                writePacketToQ(
                        data::IbusPacket(packetizer->getPacketBytes()),
                        toQ,
                        toQName
                );
                packetizer->reset(); //TODO seems we miss the first byte after a reset?
            }
        }
    }


    void SingleCoreDmaManager::handleRxInterruptServiceRoutine(
            uart_inst_t *uart,
            queue_t *toQ
    ) {
        //https://github.com/raspberrypi/pico-examples/blob/master/uart/uart_advanced/uart_advanced.c
        while (uart_is_readable(uart)) {
            uint8_t ch;// = uart_getc(uart);
            //uart_getc(uart) in the standard library casts to `char`, which might be signed.
            uart_read_blocking(uart, &ch, 1);
            if (!queue_try_add(toQ, &ch)) {
                //We couldn't write the queue?
                //Yolo-writestatus
//                writeStatus(staticLogger);
                while(!queue_is_empty(toQ)){
                    std::array<uint8_t, 255> temp = std::array<uint8_t , 255>();
                    queue_try_remove(toQ, temp.data());
                }
//                writeStatus(staticLogger);
            }
        }
    }

    void SingleCoreDmaManager::writeOutgoingQ_toUart(
            queue_t *movePacketFrom,
            uart_inst_t *uart,
            std::string fromName,
            std::string uartName) {

        //take the 255-byte buffer and parse it into a packet
        //then, if it's valid,
        //then do a uart send_blocking for len == packet.rawBytes().length
        //that way we don't clog the bus sending 0's.

        std::array<uint8_t, 255> outgoingPacketBuffer = std::array<uint8_t , 255>();
        outgoingPacketBuffer.fill(0);

        if (queue_try_remove(movePacketFrom, outgoingPacketBuffer.data())) {
            logger->d("DmaManager", fmt::format("We have a packet from {} to write to uart {}", fromName, uartName));
            auto* packet = new data::IbusPacket(outgoingPacketBuffer);
            if (packet->isPacketValid()) {
                uart_write_blocking(uart, packet->getRawPacket().data(), packet->getRawPacket().size());
                logger->d("DmaManager",
                          fmt::format("Wrote packet from {} to write to uart {}", fromName, uartName));
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

        std::array<uint8_t, 255> buffer = std::array<uint8_t , 255>();

        bool havePacket = queue_try_remove(queue, (void*) buffer.data());
        if (havePacket) {

            //A packet came into the pico for processing.
            logger->d("SingleCoreDmaManager", "Dispatching packet to cpu0 observers");
            observerRegistry->dispatchMessageToAllObservers(
                    data::IbusPacket(buffer)
            );
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
                "ToCarQ",
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
        logger->d(
                "SingleCoreDmaManager",
                fmt::format(
                        "Cpu1 Queue Health:"
                        "toPiQ: {}, "
                        "toCarQ: {}, "
                        "fromPiQ: {}, "
                        "fromCarQ: {}, ",
                        queue_get_level(&toPiQ),
                        queue_get_level(&toCarQ),
                        queue_get_level(&fromPiQ),
                        queue_get_level(&fromCarQ)
                ));
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) {
//        writePacketToQ(
//                logger,
//                packet,
//                &toPiQ,
//                "toPiQ"
//        );
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingMessage(data::IbusPacket packet) {
//        writePacketToQ(
//                logger,
//                packet,
//                &toCarQ,
//                "toCarQ"
//        );
    }

    void SingleCoreDmaManager::writePacketToQ(
            data::IbusPacket packet,
            queue_t *toQ,
            std::string toQName
    ) {
        staticLogger->d("SingleCoreDmaManager", fmt::format("Writing packet to q {}", toQName));

        std::vector<uint8_t> rawPacket = packet.getRawPacket();

        std::array<uint8_t, 255> outgoingPacketBuffer = std::array<uint8_t , 255>();
        outgoingPacketBuffer.fill(0);

        uint8_t index = 0;
        for (uint8_t byte : rawPacket) {
            outgoingPacketBuffer[index++] = byte;
        }

        bool wasAdded = queue_try_add(toQ, outgoingPacketBuffer.data());

        if (!wasAdded) {
            staticLogger->w("SingleCoreDmaManager", fmt::format("Outgoing Q {} was full, clearing", toQName));
            writeStatus(logger);
            while(!queue_is_empty(toQ)){
                std::array<uint8_t, 255> temp = std::array<uint8_t , 255>();
                queue_try_remove(toQ, temp.data());
            }
            staticLogger->d("SingleCoreDmaManager", fmt::format("Tried emptying Q {}", toQName));
            writeStatus(logger);
        }
    }


} // dma