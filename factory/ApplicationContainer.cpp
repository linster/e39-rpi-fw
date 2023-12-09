//
// Created by stefan on 12/2/22.
//

#include "ApplicationContainer.h"
#include <libs/fmt/include/fmt/format.h>
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


        sleep_ms(100);


        dmaManager->cpu0setup();

        logger->i("onMain", "Starting Linster OS Automotive");
        observerRegistry->printRegisteredObserverTags();


        pi4PowerSwitchManager->setPower(false);
        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PICO);

        logger->d("onMain",
                  fmt::format("ScanProgramManager not setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));

        scanProgramManager->cpu0setup();
        scanProgramManager->swapTo(ScanProgram::BOOT_SPLASH);

        logger->d("onMain",
                  fmt::format("ScanProgramManager is setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));

        logger->d("onMain", "Finished onMain()");
    }

    void ApplicationContainer::onLoop() {
        dmaManager->onCpu0Loop();
        scanProgramManager->onCpu0Loop();
    }

    void ApplicationContainer::onCpu1Main() {
        logger->i("onCpu1Main", "onCpu1Main");
        dmaManager->cpu1Setup(); //NOOP with SingleCoreDmaManager
        logger->i("onCpu1Main", "dmaManager->cpu1Setup() done");

        logger->i("onCpu1Main", "scanProgramManager->cpu1setup()");
        scanProgramManager->cpu1setup();
        logger->i("onCpu1Main", "scanProgramManager->cpu1setup() done");
    }

    void ApplicationContainer::onCpu1Loop() {
        dmaManager->onCpu1Loop(); //NOOP with SingleCoreDmaManager
        scanProgramManager->onCpu1Loop();
        //Let's make the LED blink while we're running this loop

    }
} // pico