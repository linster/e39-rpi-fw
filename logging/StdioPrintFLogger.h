//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_STDIOPRINTFLOGGER_H
#define PICOTEMPLATE_STDIOPRINTFLOGGER_H

#include "BaseLogger.h"

namespace pico {
    namespace logger {
        class StdioPrintFLogger : public BaseLogger {

        public:
            void print(pico::logger::BaseLogger::Level level, std::string tag,
                                          std::string message) override;
        };
    } // pico
} // logger

#endif //PICOTEMPLATE_STDIOPRINTFLOGGER_H
