//
// Created by stefan on 12/10/22.
//

#ifndef PICOTEMPLATE_IBUSLOGGEROUTPUT_H
#define PICOTEMPLATE_IBUSLOGGEROUTPUT_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>
#include <functional>

namespace pico::ibus::output::writer {

    class IbusLoggerOutput : public BaseOutputWriter {

        //Notably, don't log anything here to prevent an infinite loop.
    private:
        int truncateLogMessagesToCharacters = 64;
        //If false, drop the remainder after truncation.
        bool splitLogMessagesAtTruncationPoint = false;

        std::function<std::shared_ptr<dma::IDmaManager>()> dmaManagerAccessor;
    public:
        explicit IbusLoggerOutput(
                std::function<std::shared_ptr<dma::IDmaManager>()> dmaManagerAccessor
        );
        void print(std::string message);
    protected:
        std::string getTag() override { return "IbusLoggerOutput"; };
        //This accessor is only called when the packet is scheduled to be written.
        std::shared_ptr<dma::IDmaManager> getDmaManager() override;
    };

} // writer

#endif //PICOTEMPLATE_IBUSLOGGEROUTPUT_H
