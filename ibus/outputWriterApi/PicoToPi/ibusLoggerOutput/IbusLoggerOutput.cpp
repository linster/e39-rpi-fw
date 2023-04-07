//
// Created by stefan on 12/10/22.
//

#include "IbusLoggerOutput.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {
                IbusLoggerOutput::IbusLoggerOutput() {}

                void IbusLoggerOutput::print(std::string message) {
                    if (splitLogMessagesAtTruncationPoint) {
                        //TODO Split into multiple messages and pump them all out.
                        //TODO
                    } else {
                        //Just take the first part of the message and print one out.
                        std::string shortened = message.substr(0, std::min((int)message.size(), truncateLogMessagesToCharacters));

                        schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                            .messageType = messages::PicoToPiMessage::MessageType::LogStatement,
                            .loggerStatement = shortened
                        });
                    }
                }
            } // pico
        } // ibus
    } // output
} // writer