//
// Created by stefan on 12/10/22.
//

#ifndef PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H
#define PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <video/scanProgram/ScanProgramSwapper.h>
#include <ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h>
#include <ibus/outputWriterApi/TestingOutputWriter.h>

namespace pico::ibus::observers {

            class TelephoneLongPressObserver : public BaseObserver{

            public:
                TelephoneLongPressObserver(
                        std::shared_ptr<logger::BaseLogger> baseLogger,
                        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                        std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager,
                        std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter
                );
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
                std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager;
                std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter;

                void onTelephoneLongPressed();
            protected:
                std::string getTag() override { return "TelephoneLongPressObserver"; };
                void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
            };

        } // observers

#endif //PICOTEMPLATE_TELEPHONELONGPRESSOBSERVER_H
