//
// Created by stefan on 11/20/22.
//

#ifndef PICOTEMPLATE_ICONFIGURATIONSTORE_H
#define PICOTEMPLATE_ICONFIGURATIONSTORE_H

#include <configuration/Configuration.h>

namespace pico {
    namespace config {

        class IConfigurationStore {
        public:
            virtual bool canReadConfiguration();
            virtual void saveConfiguration(Configuration configuration);
            virtual Configuration getConfiguration();
        };

    } // pico
} // config

#endif //PICOTEMPLATE_ICONFIGURATIONSTORE_H
