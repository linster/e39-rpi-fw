//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H
#define PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H

#include <configuration/Configuration.h>

namespace pico::config::defaults {

    class DefaultConfigurationProvider {
    public:
        DefaultConfigurationProvider() = default;

        virtual Configuration getDefaultConfiguration() = 0;
    };

} // defaults

#endif //PICOTEMPLATE_DEFAULTCONFIGURATIONPROVIDER_H
