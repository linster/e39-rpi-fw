//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H
#define PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H

#include "../../BaseOutputWriter.h"

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class SoftPowerRequestWriter : BaseOutputWriter {
                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                public:
                    SoftPowerRequestWriter(
                            std::shared_ptr<logger::BaseLogger> logger
                    );

                    void requestRpiRestart();
                    void requestRpiRestartX();
                protected:
                    std::string getTag() override { return "SoftPowerRequestWriter"; };
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_SOFTPOWERREQUESTWRITER_H
