//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_IGNITIONSTATEOBSERVER_H
#define PICOTEMPLATE_IGNITIONSTATEOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <hardware/pi4powerswitch/IPi4PowerSwitchManager.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <video/scanProgram/ScanProgramSwapper.h>
#include <ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h>
namespace pico::ibus::observers {

            class IgnitionStateObserver : public BaseObserver{

            public:
                IgnitionStateObserver(
                        std::shared_ptr<logger::BaseLogger> baseLogger,
                        std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                        std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager
                );
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
                std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager;
                //https://github.com/piersholt/wilhelm-docs/blob/master/ike/11.md
                void onIgnitionKeyPosition(int position);

                bool ignoreFutureRealIgnitionEmissions = false;
            protected:
                std::string getTag() override { return "IgnitionStateObserver"; };
                void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
            };

        } // observers

#endif //PICOTEMPLATE_IGNITIONSTATEOBSERVER_H
