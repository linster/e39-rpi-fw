//
// Created by stefan on 12/2/22.
//

#include "IgnitionStateObserver.h"
#include "fmt/format.h"

namespace pico {
    namespace ibus {
        namespace observers {

            IgnitionStateObserver::IgnitionStateObserver(
                    std::shared_ptr<logger::BaseLogger> baseLogger,
                    std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                    std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                    std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                    std::shared_ptr<pico::ibus::output::writer::ScreenPowerManager> screenPowerManager
                    ) {
                this->logger = baseLogger;
                this->pi4PowerSwitchManager = pi4PowerSwitchManager;
                this->videoSwitch = videoSwitch;
                this->scanProgramSwapper = scanProgramSwapper;
                this->screenPowerManager = screenPowerManager;
                logger->d(getTag(), "Constructed");
            }


            void IgnitionStateObserver::onNewPacket(pico::ibus::data::IbusPacket iBusPacket) {

                //https://github.com/piersholt/wilhelm-docs/blob/master/ike/11.md
                //80 04 BF 11 00 2A   # KL-30  -- position 0
                //80 04 BF 11 01 2B   # KL-R   -- position 1
                //80 04 BF 11 03 29   # KL-15  -- position 2
                //80 04 BF 11 07 2D   # KL-50  -- position 3


                if (iBusPacket.getSourceDevice() == data::IbusDeviceEnum::IKE && iBusPacket.getDestinationDevice() == 0xBF) {

                    if (iBusPacket.getData()->size() == 2) {
                        if ((*iBusPacket.getData())[0] == 0x11) {
                            uint8_t position = (*iBusPacket.getData())[1];
                            switch (position) {
                                case 0x00:
                                    onIgnitionKeyPosition(0);
                                    break;
                                case 0x01:
                                    onIgnitionKeyPosition(1);
                                    break;
                                case 0x03:
                                    onIgnitionKeyPosition(2);
                                    break;
                                case 0x07:
                                    onIgnitionKeyPosition(3);
                                    break;
                            }
                        }
                    }
                }

            }

            void IgnitionStateObserver::onIgnitionKeyPosition(int position) {
                logger->d(getTag(), fmt::format("onIgnitionKeyPosition() position {:d}", position));
                switch (position) {
                    case 0:
                    //Turn the pi off
                    // (but this is a hard-shutdown, so the pi also needs to start shutting down on position 1)
                    //TODO maybe put a delay here?
                    pi4PowerSwitchManager->setPower(false);

                    videoSwitch->switchTo(hardware::videoSwitch::VideoSource::UPSTREAM);
                    screenPowerManager->sendScreenPowerMessage(false);
                    //https://github.com/piersholt/wilhelm-docs/blob/master/bmbt/4f.md
                    //TODO change the scanProgram to the clock
                    //TODO also write the message to turn on the BMBT screen
                    //TODO emit the announce messages (if we are simulating a GT)
                    break;
                    case 1:
                        //Turn the pi on
                        pi4PowerSwitchManager->setPower(true);
                        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PICO);
                        scanProgramSwapper->swapTo(ScanProgram::CLOCK);
                        screenPowerManager->sendScreenPowerMessage(true);
                        //TODO The PI will need to monitor for a 2->1 transition state,
                        //TODO and if it happens, start halting right away.
                    break;
                    case 2:
                        //The user is in a hurry to bootup, so show them the pi loading.
                        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PI);
                    break;
                }
            }



        } // pico
    } // ibus
} // observers