//
// Created by stefan on 10/24/23.
//

#include "SingleCoreDmaManager.h"
#include <algorithm>
#include <cstring>


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

        //The packet queues each get their own spin lock instead of one of the 8 shared ("striped")
        //ones. queue_try_add/remove copy a 255-byte element with that lock held and interrupts off,
        //which measured 15-29 us. Shared locks are also used by mutexes core 1 takes on every
        //scanline (CommandProcessor::renderMutex). When core 1 hit a lock core 0 was holding for one
        //of these copies, it spun with interrupts off, scanvideo's line-start interrupt (~4.5 us
        //budget) ran late, and the line started late. That set off scanvideo's scanline "recovery",
        //which could wedge the scanline state machine (see recoverWedgedScanlineStateMachine).
        //Everything for the Pi except log lines (forwarded car packets such as BMBT knob events,
        //heartbeat/config replies). 32 slots (8.4 KB) absorbs bursts; log lines can't crowd these
        //out any more because they have their own queue below (issue #18).
        queue_init_with_spinlock(
                &toPiQ,
                255,
                32,
                spin_lock_claim_unused(true)
                );

        //Log lines for the Pi, sent only while toPiQ is empty and dropped first. In CAR_WITH_PI the
        //Pi link is 9600 baud (~11 log lines a second), so boot logging always overflows something:
        //now it's only log lines.
        queue_init_with_spinlock(
                &toPiLogQ,
                255,
                16,
                spin_lock_claim_unused(true)
                );

        queue_init_with_spinlock(
                &toCarQ,
                255,
                32, //Room for a burst from the Pi; the car bus is slow (~11 ms per 10-byte packet + gaps)
                spin_lock_claim_unused(true)
                );

        queue_init_with_spinlock(
                &fromCarQ,
                255,
                10,
                spin_lock_claim_unused(true)
                );
        queue_init_with_spinlock(
                &fromPiQ,
                255,
                10,
                spin_lock_claim_unused(true)
                );

        staticLogger = logger;

        critical_section_init(&packetizerCs);
    }

    void SingleCoreDmaManager::cpu0setup() {
        setupUarts();
    }

    void SingleCoreDmaManager::onCpu0Loop() {
        messagePumpRunning = true;

#if E39_BENCH_TX_TEST
        if (benchInjectFromPiLen != 0) {
            //Start the Pi-side packetizer clean, so a stray byte already on the Pi link (e.g. a glitch
            //while the Pi is powered off) can't swallow the injected packet into a bogus frame.
            flushUart1ByteBufferToPacketizer();
            critical_section_enter_blocking(&packetizerCs);
            fromPiQPacketizer.reset();
            critical_section_exit(&packetizerCs);
            for (uint i = 0; i < benchInjectFromPiLen; i++) {
                uint8_t b = benchInjectFromPi[i];
                queue_try_add(&uart1rxByteQ, &b);
            }
            benchInjectFromPiLen = 0;
        }
#endif

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


        if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
            //This will cause printf's from the StdioPrintFlogger to also show up
            //on the UART, as well as writes from this class.

            #if (CMAKE_BUS_TOPOLOGY == CMAKE_BUS_TOPOLOGY_SLED_NO_PI)
                stdio_uart_init_full(uart1, 115200, UART1_PICOPROBE_TX, UART1_PICOPROBE_RX);
            #endif
        }

        if (busTopologyManager->getBusToplogy() == topology::BusTopology::CAR_WITH_PI  ||
            busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_LAPTOP_HMI) {
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
        handleRxInterruptServiceRoutine(uart1, &uart1rxByteQ);
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

        //Drain everything the ISR has buffered so the byte queue can't back up.
        while (queue_try_remove(fromByteQ, &byte)) {
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
            if (uart == uart0) {
                //Car bus: remember when the bus was last busy, and copy our own echo while checking a transmit.
                lastCarRxUs = time_us_32();
                if (carEchoCapture && carEchoLen < sizeof(carEchoBuffer)) {
                    carEchoBuffer[carEchoLen] = ch;
                    carEchoLen = carEchoLen + 1;
                }
            }
            //uart_getc(uart) in the standard library casts to `char`, which might be signed.
//            uart_read_blocking(uart, &ch, 1);
            if (!queue_try_add(toQ, &ch)) {
                //Queue is full. Drop the byte; the packetizer will resync.
                //Never queue_free() here: it frees the backing storage while
                //the queue is still in use, which corrupts the heap.
                rxBytesDropped = rxBytesDropped + 1;
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

        if (queue_try_remove(movePacketFrom, (void*)outgoingPacketBuffer.data())) {


            if (busTopologyManager->getBusToplogy() == topology::BusTopology::SLED_NO_PI) {
                //Guard to prevent infinite logging loop on topologies that output log messages over ibus.
                logger->d("DmaManager",
                          fmt::format("We have a packet from {} to write to uart {}", fromName, uartName));
            }

            //Packet is src, len, dest, data..., crc = len + 2 bytes. Only send it if that fits the
            //buffer (len 254/255 used to read 1-2 bytes past this stack array onto the bus) and
            //covers at least dest + crc (len 0, e.g. an empty/unparseable IbusPacket, used to
            //put a bare "00 00" on the bus).
            uint8_t len = outgoingPacketBuffer[1];
            if (len < data::IbusPacket::MIN_LENGTH_FIELD || len > data::IbusPacket::MAX_LENGTH_FIELD) {
                malformedOutgoingPacketsDropped = malformedOutgoingPacketsDropped + 1;
                return;
            }

            //TODO what if we don't construct the packet all over again? What if we just copy out the buffer dumbly?
            uart_write_blocking(uart, outgoingPacketBuffer.data(), (size_t) len + 2);
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

        //Drain all complete packets, since draining the byte queues fully can
        //produce several packets in one loop iteration.
        while (queue_try_remove(queue, (void*) buffer.data())) {
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
        writeOutgoingQ_toCarBus();
    }

    //Send the next queued car packet, with I/K-bus etiquette: only when the bus has been idle for
    //CAR_BUS_IDLE_US, at least CAR_TX_MIN_GAP_US after our previous packet, and check the echo for a
    //collision (resend after a random backoff, up to CAR_TX_MAX_ATTEMPTS). The packet stays in the
    //queue (peeked, not removed) until it's sent or given up on, so the main loop never blocks waiting
    //for the bus. This used to uart_write_blocking() every packet immediately, colliding with other
    //modules on a busy bus.
    void SingleCoreDmaManager::writeOutgoingQ_toCarBus() {
        if ((int32_t)(time_us_32() - carTxNotBeforeUs) < 0) {
            return; //spacing after our last packet, or collision backoff
        }

        std::array<uint8_t, 255> packet = std::array<uint8_t, 255>();
        if (!queue_try_peek(&toCarQ, packet.data())) {
            return;
        }

        uint8_t len = packet[1];
        if (len < data::IbusPacket::MIN_LENGTH_FIELD || len > data::IbusPacket::MAX_LENGTH_FIELD) {
            queue_try_remove(&toCarQ, packet.data());
            malformedOutgoingPacketsDropped = malformedOutgoingPacketsDropped + 1;
            return;
        }
        size_t size = (size_t) len + 2;

        if (uart_is_readable(uart0) || (time_us_32() - lastCarRxUs) < CAR_BUS_IDLE_US) {
            carTxBusyDeferrals = carTxBusyDeferrals + 1;
            return; //someone is talking (or just stopped); try again next loop
        }

        carEchoLen = 0;
        carEchoCapture = true;
        uart_write_blocking(uart0, packet.data(), size);
        uart_tx_wait_blocking(uart0);
        //The RX interrupt may only fire on its FIFO timeout (32 bit times, ~3.3 ms), so allow up to 8 ms.
        uint32_t waitStart = time_us_32();
        while (carEchoLen < size && (time_us_32() - waitStart) < 8000) {
            tight_loop_contents();
        }
        carEchoCapture = false;

        bool sentIntact = carEchoLen == size && memcmp(carEchoBuffer, packet.data(), size) == 0;
        if (sentIntact) {
            queue_try_remove(&toCarQ, packet.data());
            carTxSent = carTxSent + 1;
            carTxAttempts = 0;
            carTxNotBeforeUs = time_us_32() + CAR_TX_MIN_GAP_US;
            return;
        }

        //Collision (or a missing echo): back off for the gap plus a pseudo-random 0-10 ms, then resend.
        carTxCollisions = carTxCollisions + 1;
        carTxAttempts++;
        uint32_t now = time_us_32();
        uint32_t jitterUs = ((now * 1103515245u + 12345u) >> 8) % 10000u;
        carTxNotBeforeUs = now + CAR_TX_MIN_GAP_US + jitterUs;
        if (carTxAttempts >= CAR_TX_MAX_ATTEMPTS) {
            queue_try_remove(&toCarQ, packet.data());
            carTxDroppedAfterRetries = carTxDroppedAfterRetries + 1;
            carTxAttempts = 0;
        }
    }

    //One packet per loop, as before. Real traffic (toPiQ) always goes first; a log line only when
    //nothing else is waiting.
    void SingleCoreDmaManager::flushToPiQToUart() {
        bool hasRealTraffic = !queue_is_empty(&toPiQ);
        writeOutgoingQ_toUart(
                hasRealTraffic ? &toPiQ : &toPiLogQ,
                uart1,
                hasRealTraffic ? "ToPiQ" : "ToPiLogQ",
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
                        "uart1rxByteQ: {:d}, "
                        "rxBytesDropped: {:d}, "
                        "oversizedPacketsDropped: {:d}, "
                        "malformedOutgoingPacketsDropped: {:d}, "
                        "carTx sent/busy/collisions/dropped: {:d}/{:d}/{:d}/{:d}, "
                        "outgoingQueueFullDrops: {:d}, piLogLinesDropped: {:d} ",
                        toPiQ_level,
                        toCarQ_level,
                        fromPiQ_level,
                        fromCarQ_level,
                        uart0rxByteQ_level,
                        uart1rxByteQ_level,
                        (uint32_t)rxBytesDropped,
                        (uint32_t)oversizedPacketsDropped,
                        (uint32_t)malformedOutgoingPacketsDropped,
                        (uint32_t)carTxSent, (uint32_t)carTxBusyDeferrals,
                        (uint32_t)carTxCollisions, (uint32_t)carTxDroppedAfterRetries,
                        (uint32_t)outgoingQueueFullDrops, (uint32_t)piLogLinesDropped
                ));
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) {
        writePacketToQ(
                packet,
                &toPiQ,
                "toPiQ"
        );
    }

    void SingleCoreDmaManager::cpu0scheduleOutgoingLogMessage(data::IbusPacket packet) {
        writePacketToQ(
                packet,
                &toPiLogQ,
                "toPiLogQ"
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

//        if (!messagePumpRunning && packet.getDestinationDevice() == data::RPI) {
//            //We're not emptying the queues yet, so no point wasting time storing
//            //chatty messages that will get deleted. Our queue depths are memory-constrained,
//            //and we can't get the queues emptying before they're filled up.
//            return;
//        }

        bool log_writePacketToQ = false;

        if (log_writePacketToQ) {
            staticLogger->d("SingleCoreDmaManager", fmt::format("Writing packet to q {}", toQName));
        }

        std::vector<uint8_t> rawPacket = packet.getRawPacket();

        std::array<uint8_t, 255> outgoingPacketBuffer = std::array<uint8_t , 255>();
        outgoingPacketBuffer.fill(0);

        if (rawPacket.size() > outgoingPacketBuffer.size()) {
            //Can't fit a queue item. Copying it used to write outgoingPacketBuffer[255] (past the
            //stack array) and then wrap the uint8_t index, corrupting the packet. Drop it.
            //Don't log here: we may be inside packetizerCs, and a log line can itself become
            //a packet that comes back through this function. Counted in writeStatus().
            oversizedPacketsDropped = oversizedPacketsDropped + 1;
            return;
        }

        std::copy(rawPacket.begin(), rawPacket.end(), outgoingPacketBuffer.begin());

        bool wasAdded = queue_try_add(toQ, outgoingPacketBuffer.data());

        if (!wasAdded) {
            //Queue full: drop this packet and count it. This used to empty the whole queue, which threw
            //away every packet waiting for the car (e.g. a burst from the Pi). Don't log here: a log
            //line is itself a packet for toPiQ, so logging while toPiQ is full would recurse.
            if (toQ == &toPiLogQ) {
                piLogLinesDropped = piLogLinesDropped + 1;
            } else {
                outgoingQueueFullDrops = outgoingQueueFullDrops + 1;
            }
        }
    }


} // dma