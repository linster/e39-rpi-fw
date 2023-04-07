//
// Created by stefan on 4/5/23.
//

#include "IBusConfigMessageStore.h"

namespace pico {
    namespace config {

        IBusConfigMessageStore::IBusConfigMessageStore(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter
                ) {
            this->logger = logger;
            this->configurationStatusWriter = configurationStatusWriter;
        }

        bool IBusConfigMessageStore::canReadConfiguration() {
            /** The Pico cannot read it's own config from IBus, the Pi must
             *  push new configuration.
             */
            return false;
        }

        void IBusConfigMessageStore::saveConfiguration(Configuration configuration) {
            //Emit an Ibus message with the configuration.
            logger->d("IBusConfigMessageStore", "Scheduling Ibus emit of configuration.");
            configurationStatusWriter->scheduleEmit(configuration.toMessage());
        }

        Configuration IBusConfigMessageStore::getConfiguration() {
            logger->wtf("IBusConfigMessageStore", "getConfiguration() should never be called.");
        }

    } // pico
} // config