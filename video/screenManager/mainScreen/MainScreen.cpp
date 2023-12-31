//
// Created by stefan on 6/18/23.
//

#include "MainScreen.h"

namespace video::ScreenManager::MainScreen {


        MainScreen::MainScreen(std::shared_ptr<pico::logger::BaseLogger> logger,
                               std::shared_ptr<pico::config::ConfigurationManager> configurationManager,
                               std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                               std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                               std::shared_ptr<pico::ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                               std::shared_ptr<pico::ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter,
                               std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper) {

            this->logger = logger;
            this->configurationManager = configurationManager;
            this->pi4PowerSwitchManager = pi4PowerSwitchManager;
            this->videoSwitch = videoSwitch;
            this->configurationStatusWriter = configurationStatusWriter;
            this->softPowerRequestWriter = softPowerRequestWriter;
            this->scanProgramSwapper = scanProgramSwapper;

            this->turnOnPi4MenuItem = std::make_shared<TurnOnPi4MenuItem>(this->logger, this->pi4PowerSwitchManager);
            this->safePi4PowerOffMenuItem = std::make_shared<SafePi4PowerOffMenuItem>(this->logger,
                                                                                      this->softPowerRequestWriter);
            this->hardPi4PowerOffMenuItem = std::make_shared<HardPi4PowerOffMenuItem>(this->logger,
                                                                                      this->pi4PowerSwitchManager);

            this->videoToRvcMenuItem = std::make_shared<VideoToRvcMenuItem>(this->logger, this->videoSwitch, this->scanProgramSwapper);
            this->videoToUpstreamMenuItem = std::make_shared<VideoToUpstreamMenuItem>(this->logger, this->videoSwitch, this->scanProgramSwapper);
            this->videoToPiMenuItem = std::make_shared<VideoToPiMenuItem>(this->logger, this->videoSwitch, this->scanProgramSwapper);
            this->configPushMenuItem = std::make_shared<ConfigPushMenuItem>(this->logger, this->configurationStatusWriter, this->configurationManager);

            this->screenItems = std::vector<std::shared_ptr<ScreenItem>>();
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->turnOnPi4MenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->safePi4PowerOffMenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->hardPi4PowerOffMenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->videoToRvcMenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->videoToUpstreamMenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->videoToPiMenuItem));
            screenItems.push_back(std::reinterpret_pointer_cast<ScreenItem>(this->configPushMenuItem));

            initialize(this->logger);
        }

        std::vector<std::shared_ptr<video::ScreenManager::ScreenItem>> MainScreen::getScreenItems() {
            return this->screenItems;
        }

        std::string MainScreen::getTitle() {
            return "Main Screen";
        }

        std::string MainScreen::getTag() {
            return "MainScreen";
        }
} // MainScreen