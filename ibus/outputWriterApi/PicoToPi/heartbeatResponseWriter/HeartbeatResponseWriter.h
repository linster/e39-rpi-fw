//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H
#define PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>
#include <logging/BaseLogger.h>

namespace pico::ibus::output::writer {

                class HeartbeatResponseWriter : public BaseOutputWriter {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<dma::IDmaManager> dmaManager;
                public:
                    HeartbeatResponseWriter(
                            std::shared_ptr<logger::BaseLogger> looger,
                            std::shared_ptr<dma::IDmaManager> dmaManager
                            );

                    virtual void sendHeartbeatResponse();
                protected:
                    std::string getTag() override { return "HeartbeatResponseWriter"; };
                    std::shared_ptr<dma::IDmaManager> getDmaManager() override;
                };

            } // writer

#endif //PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H
