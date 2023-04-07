//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H
#define PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H

#include "../IConfigurationStore.h"
namespace pico {
    namespace config {

        class MemoryConfigurationStore : public IConfigurationStore {
        private:
            Configuration inMemoryConfiguration;
            bool hasConfiguration = false;
        public:
            MemoryConfigurationStore();
            bool canReadConfiguration() override;
            Configuration getConfiguration() override;
            void saveConfiguration(Configuration configuration) override;
        };

    } // pico
} // config

#endif //PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H
