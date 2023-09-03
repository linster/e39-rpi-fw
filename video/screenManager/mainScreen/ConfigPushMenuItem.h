//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_CONFIGPUSHMENUITEM_H
#define PICOTEMPLATE_CONFIGPUSHMENUITEM_H

#include <memory>
#include "../ScreenItem.h"
#include "../../../ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h"
#include "../../../configuration/ConfigurationManager.h"

namespace video::ScreenManager::MainScreen {

            class ConfigPushMenuItem : public ScreenItem {

            private:
                std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter;
                std::shared_ptr<pico::config::ConfigurationManager> configurationManager;

            public:
                ConfigPushMenuItem(
                        std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                        std::shared_ptr<pico::config::ConfigurationManager> configurationManager
                        );

                std::string getLabel() override;
                void onItemClicked() override;
            };

} // MainScreen

#endif //PICOTEMPLATE_CONFIGPUSHMENUITEM_H
