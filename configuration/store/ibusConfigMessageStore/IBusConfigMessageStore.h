//
// Created by stefan on 4/5/23.
//

#ifndef PICOTEMPLATE_IBUSCONFIGMESSAGESTORE_H
#define PICOTEMPLATE_IBUSCONFIGMESSAGESTORE_H

#include <configuration/store/IConfigurationStore.h>
#include <logging/BaseLogger.h>
#include <ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h>
namespace pico::config {

        class IBusConfigMessageStore : public IConfigurationStore {

        private:
            std::shared_ptr<logger::BaseLogger> logger;
            std::shared_ptr<ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter;
        public:
            IBusConfigMessageStore(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter
                    );

            Configuration* getConfiguration() override;
            bool canReadConfiguration() override;

            /** Emit an IBus message with the current config. */
            void saveConfiguration(Configuration configuration) override;
        };

    } // config

#endif //PICOTEMPLATE_IBUSCONFIGMESSAGESTORE_H
