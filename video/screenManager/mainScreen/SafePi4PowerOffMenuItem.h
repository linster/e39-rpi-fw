//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_SAFEPI4POWEROFFMENUITEM_H
#define PICOTEMPLATE_SAFEPI4POWEROFFMENUITEM_H

#include <memory>
#include "../../../ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"
#include "../ScreenItem.h"

namespace video::ScreenManager::MainScreen {

            class SafePi4PowerOffMenuItem : ScreenItem {


            private:
                std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter;
            public:

                SafePi4PowerOffMenuItem(
                        std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter
                        );

                std::string getLabel() override;
                void onItemClicked() override;

            };

        } // MainScreen

#endif //PICOTEMPLATE_SAFEPI4POWEROFFMENUITEM_H
