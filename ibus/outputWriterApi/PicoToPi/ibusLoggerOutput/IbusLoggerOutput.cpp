//
// Created by stefan on 12/10/22.
//

#include "IbusLoggerOutput.h"
namespace pico::ibus::output::writer {
    IbusLoggerOutput::IbusLoggerOutput(
            std::function<dma::IDmaManager*()> dmaManagerAccessor
    ) {
        this->dmaManagerAccessor = dmaManagerAccessor;
    }

    std::shared_ptr<dma::IDmaManager> IbusLoggerOutput::getDmaManager() {
        //This writer runs on both cores (core 1 logs too), so it must not copy the DMA manager's
        //shared_ptr: the reference count isn't atomic. BaseOutputWriter uses
        //getDmaManagerForWrite() below instead.
        return nullptr;
    }

    dma::IDmaManager* IbusLoggerOutput::getDmaManagerForWrite() {
        return dmaManagerAccessor();
    }

    void IbusLoggerOutput::print(std::string message) {

        if (dmaManagerAccessor() == nullptr) {
            //We cannot log anything here until the dma manager has been set up.
            return;
        }

        if (splitLogMessagesAtTruncationPoint) {
            //TODO Split into multiple messages and pump them all out.
            //TODO
        } else {
            //Just take the first part of the message and print one out.
            std::string shortened = message.substr(0, std::min((int)message.size(), truncateLogMessagesToCharacters));

            schedulePicoToPiMessageForWrite(messages::PicoToPiMessage {
                .messageType = messages::PicoToPiMessage::MessageType::LogStatement,
                .loggerMessage = shortened
            });
        }
    }
} // writer