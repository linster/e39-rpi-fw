//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_SCREENPOWERMANAGER_H
#define PICOTEMPLATE_SCREENPOWERMANAGER_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>
#include <logging/BaseLogger.h>
#include <configuration/Configuration.h>
#include <configuration/default/PimoroniDefaultConfigurationProvider.h>
#include <libs/fmt/include/fmt/format.h>

namespace pico {
    namespace ibus {
        namespace output {
            namespace writer {

                class ScreenPowerManager : public BaseOutputWriter {

                private:
                    std::shared_ptr<logger::BaseLogger> logger;
                    std::shared_ptr<config::defaults::PimoroniDefaultConfigurationProvider> defaultConfigurationProvider;
                    std::shared_ptr<pico::config::Configuration> defaultConfiguration;
                    std::shared_ptr<dma::IDmaManager> dmaManager;
                    //https://github.com/piersholt/wilhelm-docs/blob/master/bmbt/4f.md
                    bool aspectRatioIs4_3;
                    bool aspectRatioIs16_9;
                    bool encodingIsNtsc;
                    bool encodingIsPal;

                    //TODO this also needs an IBUS observer to set this to false if any
                    //TODO other device sets this to off so that we can remember to turn
                    //TODO the screen off after we're done backing up, if it was off before that.
                    bool isPowerOn;
                    void logCurrentState(std::string subTag);
                    void logNewState(std::string subTag, bool newState);
                public:
                    ScreenPowerManager(
                            std::shared_ptr<config::defaults::PimoroniDefaultConfigurationProvider> defaultConfigurationProvider,
                            std::shared_ptr<logger::BaseLogger> baseLogger,
                            std::shared_ptr<dma::IDmaManager> dmaManager
                            );

                    std::string getTag() override;

                    void setScreenPowerStateFromObservedIbusMessage(bool isOn);
                    void sendScreenPowerMessage(bool isOn);

                    void sendScreenInitializationMessage();

                protected:
                    std::shared_ptr<dma::IDmaManager> getDmaManager() override;
                };

            } // pico
        } // ibus
    } // output
} // writer

#endif //PICOTEMPLATE_SCREENPOWERMANAGER_H
