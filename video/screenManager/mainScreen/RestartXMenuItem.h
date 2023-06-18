//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_RESTARTXMENUITEM_H
#define PICOTEMPLATE_RESTARTXMENUITEM_H

#include <memory>
#include "../ScreenItem.h"
#include "../../../ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"

namespace video::ScreenManager::MainScreen {

            class RestartXMenuItem : ScreenItem {

            private:
                std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter;
            public:
                RestartXMenuItem(
                        std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter
                );

                std::string getLabel() override;
                void onItemClicked() override;
            };

        } // MainScreen

#endif //PICOTEMPLATE_RESTARTXMENUITEM_H
