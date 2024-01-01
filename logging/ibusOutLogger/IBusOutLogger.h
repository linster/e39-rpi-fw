//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_IBUSOUTLOGGER_H
#define PICOTEMPLATE_IBUSOUTLOGGER_H

#include <memory>
#include "../BaseLogger.h"
#include "../../ibus/outputWriterApi/PicoToPi/ibusLoggerOutput/IbusLoggerOutput.h"
namespace pico::logger::ibus {

            class IBusOutLogger : public BaseLogger {
            private:
                std::shared_ptr<pico::ibus::output::writer::IbusLoggerOutput> loggerOutput;
            protected:
                void print(Level level, std::string tag, std::string message) override;
            };

        } // ibus

#endif //PICOTEMPLATE_IBUSOUTLOGGER_H
