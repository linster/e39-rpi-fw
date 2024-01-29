//
// Created by stefan on 1/1/24.
//

#include "LoggerFactory.h"


namespace pico::logger::factory {

    LoggerFactory::LoggerFactory(
            std::function<std::shared_ptr<pico::ibus::dma::IDmaManager>()> dmaManagerAccessor,
            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager
    ) {
        this->dmaManagerAccessor = dmaManagerAccessor;
        this->busTopologyManager = busTopologyManager;
    }

    std::shared_ptr<BaseLogger> LoggerFactory::buildAndGetLogger() {
        if (this->returnedBaseLogger.get() == nullptr) {
            switch (busTopologyManager->getBusToplogy()) {
                case pico::ibus::topology::CAR_WITH_PI:
                case pico::ibus::topology::SLED_LAPTOP_HMI:
                    //We want only ibus raw on the uart
                    //STDIO is setup to be on USB, not on uart.
                    logsToStdioAndUart();
                    break;
                case pico::ibus::topology::SLED_NO_PI:
                    //We want to print a bunch of junk to the Uart, all our log
                    //messages end up in plain text on the uart, jumbled with
                    //raw packets. It is okay to listen to this on a serial monitor,
                    //but impossible to use with e39-rpi, or anything expecting raw ibus
                    //packets.
                    logsToStdio();
                    break;
            }
        }
        return this->returnedBaseLogger;
    }

    void LoggerFactory::logsToStdio() {
        //We want to print out all the logs to stdout, because stdout is setup to go over UART1,
        //which goes to the picoprobe, which can be read from the laptop.
        this->returnedBaseLogger = std::make_shared<logger::StdioPrintFLogger>();
    }

    void LoggerFactory::logsToStdioAndUart() {
        //All logs get queued up as ibus messages.
        //Print out logs to stdout too, since that's attached to USB stdio and not to UART1.

        this->stdioPrintFLogger = std::make_shared<StdioPrintFLogger>();

        this->loggerOutput = std::make_shared<pico::ibus::output::writer::IbusLoggerOutput>(this->dmaManagerAccessor);
        this->iBusOutLogger = std::make_shared<pico::logger::ibus::IBusOutLogger>(this->loggerOutput);

        std::vector<std::shared_ptr<BaseLogger>> loggerList;
        loggerList.push_back(this->stdioPrintFLogger);
        loggerList.push_back(this->iBusOutLogger);

        this->returnedBaseLogger = std::make_shared<CompositeLogger>(loggerList);
    }

    void LoggerFactory::logsToUart() {
        this->loggerOutput = std::make_shared<pico::ibus::output::writer::IbusLoggerOutput>(this->dmaManagerAccessor);
        this->returnedBaseLogger = std::make_shared<ibus::IBusOutLogger>(this->loggerOutput);
    }
} // factory