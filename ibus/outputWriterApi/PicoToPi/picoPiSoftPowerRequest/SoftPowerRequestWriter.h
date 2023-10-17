//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H
#define PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class SoftPowerRequestWriter : BaseOutputWriter {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<dma::DmaManager> dmaManager;
                public:
                    SoftPowerRequestWriter(
                            std::shared_ptr<logger::BaseLogger> logger,
                            std::shared_ptr<dma::DmaManager> dmaManager
                    );

                    void requestRpiRestart();
                    void requestRpiRestartX();
                protected:
                    std::string getTag() override { return "SoftPowerRequestWriter"; };
                    std::shared_ptr<dma::DmaManager> getDmaManager() override;
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H
