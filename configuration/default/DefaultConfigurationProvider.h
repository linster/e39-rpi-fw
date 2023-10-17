//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H
#define PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H

#include <configuration/Configuration.h>

namespace pico {
    namespace config {
        namespace defaults {

            class DefaultConfigurationProvider {
            public:
                DefaultConfigurationProvider() = default;
                virtual Configuration getDefaultConfiguration() = 0;
            };

        } // pico
    } // config
} // defaults

#endif //PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H
