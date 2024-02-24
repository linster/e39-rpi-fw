//
// Created by stefan on 12/2/22.
//

#include "ApplicationContainerNoVideo.h"
#include <libs/fmt/include/fmt/format.h>
#include "pico/time.h"

namespace pico {
    void ApplicationContainerNoVideo::onMain() {

        logger->d("onMain", "onMain");

        watchdogManager->cpu0setup();

        //Register all the observers to the observer registry.
        for (auto & element : *baseObservers) {
            logger->d("onMain", fmt::format("Registering observer: {}", element->getTag()));
            observerRegistry->registerObserver(element);
        }
        logger->d("onMain", "Registered all observers to ObserverRegistry");


        sleep_ms(100);


        dmaManager->cpu0setup();

        logger->i("onMain", "Starting Linster OS Automotive");

        switch (busTopologyManager->getBusToplogy()) {
            case ibus::topology::CAR_WITH_PI:
                logger->i("onMain", "Bus topology is CAR_WITH_PI");
                break;
            case ibus::topology::SLED_NO_PI:
                logger->i("onMain", "Bus topology is SLED_NO_PI");
                break;
            case ibus::topology::SLED_LAPTOP_HMI:
                logger->i("onMain", "Bus topology is SLED_LAPTOP_HMI");
                break;
        }

        observerRegistry->printRegisteredObserverTags();

        pi4PowerSwitchManager->setPower(false);

        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PI);

        logger->d("onMain", "Finished onMain()");
    }

    void ApplicationContainerNoVideo::onLoop() {
        watchdogManager->onCpu0Loop();
        dmaManager->onCpu0Loop();
    }

    void ApplicationContainerNoVideo::onCpu1Main() {
//        multicore_lockout_victim_init();
        logger->i("onCpu1Main", "onCpu1Main");
        watchdogManager->cpu1Setup();
        dmaManager->cpu1Setup(); //NOOP with SingleCoreDmaManager
        logger->i("onCpu1Main", "dmaManager->cpu1Setup() done");
    }

    void ApplicationContainerNoVideo::onCpu1Loop() {
        watchdogManager->onCpu1Loop();
        dmaManager->onCpu1Loop();
    }
} // pico