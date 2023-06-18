//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H
#define PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H

#include "../../BaseOutputWriter.h"
#include "../../../../proto_cpp/ConfigMessage.h"
#include "../../../../logging/BaseLogger.h"
namespace pico::ibus::output::writer {

                class ConfigurationStatusWriter : public BaseOutputWriter {

                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<dma::DmaManager> dmaManager;
                public:
                    //TODO add a logger in here.
                    ConfigurationStatusWriter(
                            std::shared_ptr<logger::BaseLogger> logger,
                            std::shared_ptr<dma::DmaManager> dmaManager
                        );
                    void scheduleEmit(messages::ConfigMessage configMessage);
                protected:
                    std::string getTag() override { return "ConfigurationStatusWriter"; };
                    std::shared_ptr<dma::DmaManager> getDmaManager() override;
                };

            } // writer

#endif //PICOTEMPLATE_CONFIGURATIONSTATUSWRITER_H
