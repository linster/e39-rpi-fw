//
// Created by stefan on 2/22/24.
//

#ifndef PICOTEMPLATE_TELEPHONEPRESSNOVIDEO_H
#define PICOTEMPLATE_TELEPHONEPRESSNOVIDEO_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h>
#include <ibus/outputWriterApi/TestingOutputWriter.h>
#include "hardware/pi4powerswitch/IPi4PowerSwitchManager.h"

namespace pico::ibus::observers {

    class TelephonePressNoVideo  : public BaseObserver {

    public:
        TelephonePressNoVideo(
                std::shared_ptr<logger::BaseLogger> baseLogger,
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager,
                std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter,
                std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager
                );

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
        std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager;
        std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter;
        std::shared_ptr<pico::hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;
        void onTelephonePressed();
        void onTelephoneLongPressed();


        void swapToNextVideoSource();
        void restartRpi();

    protected:
        std::string getTag() override { return "TelephonePressNoVideo"; };
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
    };

} // observers

#endif //PICOTEMPLATE_TELEPHONEPRESSNOVIDEO_H
