//
// Created by stefan on 12/10/22.
//

#ifndef PICOTEMPLATE_IBUSLOGGEROUTPUT_H
#define PICOTEMPLATE_IBUSLOGGEROUTPUT_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class IbusLoggerOutput : public BaseOutputWriter {

                    //Notably, don't log anything here to prevent an infinite loop.
                private:
                    int truncateLogMessagesToCharacters = 80;
                    //If false, drop the remainder after truncation.
                    bool splitLogMessagesAtTruncationPoint = false;

                    std::shared_ptr<dma::IDmaManager> dmaManager;
                public:
                    IbusLoggerOutput(
                            std::shared_ptr<dma::IDmaManager> dmaManager
                    );
                    void print(std::string message);
                protected:
                    std::string getTag() override { return "IbusLoggerOutput"; };
                    std::shared_ptr<dma::IDmaManager> getDmaManager() override;
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_IBUSLOGGEROUTPUT_H
