//
// Created by stefan on 12/10/22.
//

#ifndef PICOTEMPLATE_IBUSLOGGEROUTPUT_H
#define PICOTEMPLATE_IBUSLOGGEROUTPUT_H

#include "../BaseOutputWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class IbusLoggerOutput : public BaseOutputWriter {

                public:
                    IbusLoggerOutput();
                    void print(std::string message);
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_IBUSLOGGEROUTPUT_H
