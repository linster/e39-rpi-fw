//
// Created by stefan on 8/22/24.
//

#ifndef PICOTEMPLATE_IGNITIONSTATUSREQUESTWRITER_H
#define PICOTEMPLATE_IGNITIONSTATUSREQUESTWRITER_H

#include "ibus/outputWriterApi/BaseOutputWriter.h"

namespace pico::ibus::output::writer::ignition {

    class IgnitionStatusRequestWriter : public BaseOutputWriter{

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<dma::IDmaManager> dmaManager;

    public:
        IgnitionStatusRequestWriter(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<dma::IDmaManager> dmaManager
                );

        void sendIgnitionStatusRequest();
    protected:
        std::string getTag() override { return "IgnitionStatusRequestWriter"; };
        std::shared_ptr<dma::IDmaManager> getDmaManager() override;
    };

} // ignition

#endif //PICOTEMPLATE_IGNITIONSTATUSREQUESTWRITER_H
