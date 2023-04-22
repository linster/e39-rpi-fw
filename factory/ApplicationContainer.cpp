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

        dmaManager->cpu0setup();
    }

    void ApplicationContainer::onLoop() {
        logger->d("onLoop", "onLoop");
//
//        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PI);
//        sleep_ms(2000);
//        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::UPSTREAM);
//
//        sleep_ms(2000);
//        pi4PowerSwitchManager->setPower(true);
//        sleep_ms(2000);
//        pi4PowerSwitchManager->setPower(false);

        dmaManager->onCpu0Loop();

        sleep_ms(2000);
        heartbeatResponseWriter->sendHeartbeatResponse();
    }

    void ApplicationContainer::onCpu1Main() {
        dmaManager->cpu1Setup();
    }

    void ApplicationContainer::onCpu1Loop() {
        dmaManager->onCpu1Loop();
    }
} // pico