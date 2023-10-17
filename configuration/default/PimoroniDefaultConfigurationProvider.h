//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_PIMORONIDEFAULTCONFIGURATIONPROVIDER_H
#define PICOTEMPLATE_PIMORONIDEFAULTCONFIGURATIONPROVIDER_H

#include "DefaultConfigurationProvider.h"
#include <proto_generated/ConfigProto.pb.h>
namespace pico {
    namespace config {
        namespace defaults {

            class PimoroniDefaultConfigurationProvider : public DefaultConfigurationProvider {
            public:
                PimoroniDefaultConfigurationProvider() = default;
                Configuration getDefaultConfiguration() override;
            };

        } // pico
    } // config
} // defaults

#endif //PICOTEMPLATE_PIMORONIDEFAULTCONFIGURATIONPROVIDER_H
