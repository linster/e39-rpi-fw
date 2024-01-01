//
// Created by stefan on 1/1/24.
//

#ifndef PICOTEMPLATE_LOGGERFACTORY_H
#define PICOTEMPLATE_LOGGERFACTORY_H

#include <memory.h>
#include "ibus/dma/topology/BusTopologyManager.h"
#include <logging/BaseLogger.h>
#include <logging/CompositeLogger.h>
#include <logging/StdioPrintFLogger.h>
#include <logging/ibusOutLogger/IBusOutLogger.h>

namespace pico::logger::factory {

    class LoggerFactory {

    private:
        std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager;

        std::shared_ptr<logger::StdioPrintFLogger> stdioPrintFLogger;

        std::function<std::shared_ptr<pico::ibus::dma::IDmaManager>()> dmaManagerAccessor;
        std::shared_ptr<pico::ibus::output::writer::IbusLoggerOutput> loggerOutput;
        std::shared_ptr<pico::logger::ibus::IBusOutLogger> iBusOutLogger;

        std::shared_ptr<BaseLogger> returnedBaseLogger;

        void buildLoggerForPi();
        void buildLoggerNoPi();

    public:

        LoggerFactory(
                std::function<std::shared_ptr<pico::ibus::dma::IDmaManager>()> dmaManagerAccessor,
                std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager
                );

        std::shared_ptr<BaseLogger> buildAndGetLogger();
    };

} // factory

#endif //PICOTEMPLATE_LOGGERFACTORY_H
