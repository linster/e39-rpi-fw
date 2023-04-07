//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H
#define PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H

#include "../../BaseOutputWriter.h"
#include "../../../../proto_cpp/ConfigMessage.h"
#include "../../../../logging/BaseLogger.h"
namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class ConfigurationStatusWriter : public BaseOutputWriter {

                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                public:
                    //TODO add a logger in here.
                    ConfigurationStatusWriter(
                            std::shared_ptr<logger::BaseLogger> logger
                        );
                    void scheduleEmit(messages::ConfigMessage configMessage);
                protected:
                    std::string getTag() override { return "ConfigurationStatusWriter"; };
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H
