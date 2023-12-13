//
// Created by stefan on 12/10/22.
//

#include "IbusLoggerOutput.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {
                IbusLoggerOutput::IbusLoggerOutput(
                        std::shared_ptr<dma::IDmaManager> dmaManager
                ) {
                    this->dmaManager = dmaManager;
                }

                std::shared_ptr<dma::IDmaManager> IbusLoggerOutput::getDmaManager() {
                    return dmaManager;
                }

                void IbusLoggerOutput::print(std::string message) {
                    if (splitLogMessagesAtTruncationPoint) {
                        //TODO Split into multiple messages and pump them all out.
                        //TODO
                    } else {
                        //Just take the first part of the message and print one out.
                        std::string shortened = message.substr(0, std::min((int)message.size(), truncateLogMessagesToCharacters));

                        schedulePicoToPiMessageForWrite(messages::PicoToPiMessage(
                                messages::PicoToPiMessage::MessageType::LogStatement,
                                shortened
                        ));
                    }
                }
            } // pico
        } // ibus
    } // output
} // writer