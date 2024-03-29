//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_PIDEFAULTCONFIGURATIONPROVIDER_H
#define PICOTEMPLATE_PIDEFAULTCONFIGURATIONPROVIDER_H

#include "DefaultConfigurationProvider.h"
#include <proto_generated/ConfigProto.pb.h>

namespace pico::config::defaults {

    class PiDefaultConfigurationProvider : public DefaultConfigurationProvider {
    public:
        PiDefaultConfigurationProvider() = default;

        Configuration getDefaultConfiguration() override;
    };

} // defaults

#endif //PICOTEMPLATE_PIDEFAULTCONFIGURATIONPROVIDER_H
