//
// Created by stefan on 12/2/22.
//

#include "ApplicationContainer.h"
#include "../libs/fmt/include/fmt/format.h"
#include "pico/time.h"

namespace pico {
    void ApplicationContainer::onMain() {

        logger->d("onMain", "onMain");

        //Register all the observers to the observer registry.
        for (auto & element : *baseObservers) {
            logger->d("onMain", fmt::format("Registering observer: {}", element->getTag()));
            observerRegistry->registerObserver(element);
        }
        logger->d("onMain", "Registered all observers to ObserverRegistry");

//        pi4PowerSwitchManager->setPower(false);

//        dmaManager->cpu0setup();

        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::RVC);

        scanProgramManager->cpu0setup();

        scanProgramSwapper->swapTo(ScanProgram::DEMO);

        videoSwitch->switchTo(hardware::videoSwitch::PICO);
    }

    void ApplicationContainer::onLoop() {
        scanProgramManager->onCpu0Loop();
//        logger->d("onLoop", "onLoop");
//        if (cpu0LoopCount % 1 == 0) {
//            logger->d("onLoop", fmt::format("Loop count {}", cpu0LoopCount++));
//        }
        //
//        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::RVC);
//        sleep_ms(4000);
//        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::UPSTREAM);
//        sleep_ms(4000);
//
//        sleep_ms(2000);
//        pi4PowerSwitchManager->setPower(true);
//        sleep_ms(2000);
//        pi4PowerSwitchManager->setPower(false);

//        dmaManager->onCpu0Loop();

        //sleep_ms(2000);
        //heartbeatResponseWriter->sendHeartbeatResponse();


    }

    void ApplicationContainer::onCpu1Main() {
        logger->i("onCpu1Main", "onCpu1Main");
        dmaManager->cpu1Setup();
        scanProgramManager->cpu1setup();
        logger->i("onCpu1Main", "dmaManager->cpu1Setup() done");
    }

    void ApplicationContainer::onCpu1Loop() {
        dmaManager->onCpu1Loop();
        scanProgramManager->onCpu1Loop();
    }
} // pico