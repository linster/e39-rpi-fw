//
// Created by stefan on 1/1/24.
//

#include "LoggerFactory.h"


namespace pico::logger::factory {

    LoggerFactory::LoggerFactory(
            std::shared_ptr<pico::ibus::dma::IDmaManager> dmaManager,
            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager
    ) {
        this->dmaManager = dmaManager;
        this->busTopologyManager = busTopologyManager;
    }

    std::shared_ptr<BaseLogger> LoggerFactory::buildAndGetLogger() {
        if (this->returnedBaseLogger.get() == nullptr) {
            switch (busTopologyManager->getBusToplogy()) {
                case pico::ibus::topology::CAR_WITH_PI:
                    buildLoggerForPi();
                    break;
                case pico::ibus::topology::SLED_NO_PI:
                    buildLoggerNoPi();
                    break;
            }
        }
        return this->returnedBaseLogger;
    }

    void LoggerFactory::buildLoggerNoPi() {
        //We want to print out all the logs to stdout, because stdout is setup to go over UART1,
        //which goes to the picoprobe, which can be read from the laptop.
        this->returnedBaseLogger = std::make_shared<logger::StdioPrintFLogger>();
    }

    void LoggerFactory::buildLoggerForPi() {
        //All logs get queued up as ibus messages.
        //Print out logs to stdout too, since that's attached to USB stdio and not to UART1.

        this->stdioPrintFLogger = std::make_shared<StdioPrintFLogger>();

        this->loggerOutput = std::make_shared<pico::ibus::output::writer::IbusLoggerOutput>(this->dmaManager);
        this->iBusOutLogger = std::make_shared<pico::logger::ibus::IBusOutLogger>(this->loggerOutput);

        std::vector<std::shared_ptr<BaseLogger>> loggerList;
        loggerList.push_back(this->stdioPrintFLogger);
        loggerList.push_back(this->iBusOutLogger);

        this->returnedBaseLogger = std::make_shared<CompositeLogger>(loggerList);
    }
} // factory