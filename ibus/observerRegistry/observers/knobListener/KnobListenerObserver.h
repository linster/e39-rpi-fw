//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_KNOBLISTENEROBSERVER_H
#define PICOTEMPLATE_KNOBLISTENEROBSERVER_H

#include <memory>
#include <logging/BaseLogger.h>
#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <video/scanProgram/ScanProgramSwapper.h>
#include <hardware/videoSwitch/VideoSwitch.h>
#include <video/screenManager/ScreenManager.h>

namespace pico::ibus::observers {

            class KnobListenerObserver : public BaseObserver{

            public:
                KnobListenerObserver(
                        std::shared_ptr<logger::BaseLogger> baseLogger,
                        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch,
                        std::shared_ptr<video::ScreenManager::ScreenManager> screenManager
                );
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;
                std::shared_ptr<video::ScreenManager::ScreenManager> screenManager;

                void onKnobTurnedRight(int clicks);
                void onKnobTurnedLeft(int clicks);
                void onKnobPressed();

                bool debugDraw = false;

                /**
                 * If true, update the current state in the screenManager when
                 * items are focused and selected, whether or not the video source
                 * or scan program are correct. Use this to develop the menu business
                 * logic before the scan program totally works.
                 */
                bool mock_knob_state_preConditions = true;
            protected:
                std::string getTag() override { return "KnobListenerObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // observers

#endif //PICOTEMPLATE_KNOBLISTENEROBSERVER_H
