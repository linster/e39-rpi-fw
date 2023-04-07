//
// Created by stefan on 4/5/23.
//

#include "ConfigurationStatusWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                ConfigurationStatusWriter::ConfigurationStatusWriter(
                        std::shared_ptr<logger:BaseLogger> logger
                ) {
                    this->logger = logger;
                }

                void ConfigurationStatusWriter::scheduleEmit(messages::ConfigMessage configMessage) {
                    //TODO wrap the backingMessage by converting to binary (proto), jam into IBus packet.
                }
            } // pico
        } // ibus
    } // output
} // writer