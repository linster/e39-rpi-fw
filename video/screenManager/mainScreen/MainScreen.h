//
// Created by stefan on 6/18/23.
//

#ifndef PICOTEMPLATE_MAINSCREEN_H
#define PICOTEMPLATE_MAINSCREEN_H

#include "../Screen.h"
#include "../../../hardware/pi4powerswitch/IPi4PowerSwitchManager.h"
#include "../../../hardware/videoSwitch/VideoSwitch.h"
#include "../../../ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h"
#include "../../../ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"
#include "TurnOnPi4MenuItem.h"
#include "SafePi4PowerOffMenuItem.h"
#include "HardPi4PowerOffMenuItem.h"
#include "RestartXMenuItem.h"
#include "VideoToRvcMenuItem.h"
#include "VideoToUpstreamMenuItem.h"
#include "VideoToPiMenuItem.h"
#include "../../../configuration/ConfigurationManager.h"
#include "ConfigPushMenuItem.h"

namespace video {
    namespace ScreenManager {
        namespace MainScreen {

            class MainScreen : Screen {

            private:
                std::shared_ptr<pico::logger::BaseLogger> logger;
                std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<pico::config::ConfigurationManager> configurationManager;
                std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter;
                std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter;


                std::shared_ptr<video::ScreenManager::MainScreen::TurnOnPi4MenuItem> turnOnPi4MenuItem;
                std::shared_ptr<video::ScreenManager::MainScreen::SafePi4PowerOffMenuItem> safePi4PowerOffMenuItem;
                std::shared_ptr<video::ScreenManager::MainScreen::HardPi4PowerOffMenuItem> hardPi4PowerOffMenuItem;
                std::shared_ptr<video::ScreenManager::MainScreen::RestartXMenuItem> restartXMenuItem;

                std::shared_ptr<video::ScreenManager::MainScreen::VideoToRvcMenuItem> videoToRvcMenuItem;
                std::shared_ptr<video::ScreenManager::MainScreen::VideoToUpstreamMenuItem> videoToUpstreamMenuItem;
                std::shared_ptr<video::ScreenManager::MainScreen::VideoToPiMenuItem> videoToPiMenuItem;

                std::shared_ptr<video::ScreenManager::MainScreen::ConfigPushMenuItem> configPushMenuItem;

                std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>> screenItems;

            public:

                MainScreen(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::config::ConfigurationManager> configurationManager,
                        std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                        std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter
                );

                std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>> getScreenItems() override;

                std::string getTitle() override;
            };

        } // video
    } // ScreenManager
} // MainScreen

#endif //PICOTEMPLATE_MAINSCREEN_H
