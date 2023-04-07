//
// Created by stefan on 12/2/22.
//

#include "IBusOutLogger.h"
#include "fmt/format.h"
namespace pico {
    namespace logger {
        namespace ibus {
            void IBusOutLogger::print(Level level, std::string tag, std::string message) {
                std::string levelString;
                switch (level) {
                    case Level::DEBUG : { levelString = "D" ; break; }
                    case Level::INFO : { levelString = "I" ; break; }
                    case Level::WARN : { levelString = "W" ; break; }
                    case Level::ERROR : { levelString = "E" ; break; }
                    case Level::WTF : { levelString = "?" ; break; }
                    default: { levelString = "" ; break; }
                }
                loggerOutput->print(fmt::format("{} {}", levelString, message));
            }
        } // pico
    } // logger
} // ibus