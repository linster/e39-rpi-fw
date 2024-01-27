//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H
#define PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H

#include <configuration/store/IConfigurationStore.h>
namespace pico::config {

        class MemoryConfigurationStore : public IConfigurationStore {
        private:
            Configuration inMemoryConfiguration = Configuration();
            bool hasConfiguration = false;
        public:
            explicit MemoryConfigurationStore();
            bool canReadConfiguration() override;
            Configuration* getConfiguration() override;
            void saveConfiguration(Configuration configuration) override;
        };

    } // config

#endif //PICOTEMPLATE_MEMORYCONFIGURATIONSTORE_H
