//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_BASELOGGER_H
#define PICOTEMPLATE_BASELOGGER_H

#include <string>

namespace pico {
    namespace logger {

        class BaseLogger {

        public:
            virtual void d(std::string tag, std::string message);
            virtual void i(std::string tag, std::string message);
            virtual void w(std::string tag, std::string message);
            virtual void e(std::string tag, std::string message);
            virtual void wtf(std::string tag, std::string message);
        protected:
            enum class Level { DEBUG, INFO, WARN, ERROR, WTF};
            virtual void print(Level level, std::string tag, std::string message) = 0;
        };

    } // pico
} // logger

#endif //PICOTEMPLATE_BASELOGGER_H
