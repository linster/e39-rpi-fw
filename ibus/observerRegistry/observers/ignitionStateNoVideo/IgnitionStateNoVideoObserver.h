//
// Created by stefan on 2/22/24.
//

#ifndef PICOTEMPLATE_IGNITIONSTATENOVIDEOOBSERVER_H
#define PICOTEMPLATE_IGNITIONSTATENOVIDEOOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <hardware/pi4powerswitch/IPi4PowerSwitchManager.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h>
#include <ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h>

namespace pico::ibus::observers {

    class IgnitionStateNoVideoObserver : public BaseObserver {

    public:
        IgnitionStateNoVideoObserver(
                std::shared_ptr<logger::BaseLogger> baseLogger,
                std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager,
                std::shared_ptr<output::writer::SoftPowerRequestWriter> softPowerRequestWriter
        );
    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;
        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
        std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager;
        std::shared_ptr<output::writer::SoftPowerRequestWriter> softPowerRequestWriter;
        //https://github.com/piersholt/wilhelm-docs/blob/master/ike/11.md
        void onIgnitionKeyPosition(int position);
        void onIgnitionKeyPositionUpdates(int oldPosition, int newPosition);
        bool ignoreFutureRealIgnitionEmissions = false;
        int currentKeyPosition = -1;

        // TODO make an ignition status request writer and call it when the pico boots up.
    protected:
        std::string getTag() override { return "IgnitionStateNoVideoObserver"; };
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
    };

} // observers

#endif //PICOTEMPLATE_IGNITIONSTATENOVIDEOOBSERVER_H
