//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H
#define PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H

#include "../../BaseOutputWriter.h"
#include "../../../logging/BaseLogger.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class HeartbeatResponseWriter : public BaseOutputWriter {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<dma::DmaManager> dmaManager;
                public:
                    HeartbeatResponseWriter(
                            std::shared_ptr<logger::BaseLogger> looger,
                            std::shared_ptr<dma::DmaManager> dmaManager
                            );

                    void sendHeartbeatResponse();
                protected:
                    std::string getTag() override { return "HeartbeatResponseWriter"; };
                    std::shared_ptr<dma::DmaManager> getDmaManager() override;
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_HEARTBEATRESPONSEWRITER_H
