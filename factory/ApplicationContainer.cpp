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


        sleep_ms(1000);


        dmaManager->cpu0setup();

        logger->i("onMain", "Starting Linster OS Automotive");

        observerRegistry->printRegisteredObserverTags();


        pi4PowerSwitchManager->setPower(false);

        videoSwitch->switchTo(hardware::videoSwitch::VideoSource::PICO);

        logger->d("onMain",
                  fmt::format("ScanProgramManager not setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));

        scanProgramManager->cpu0setup();

        scanProgramManager->swapTo(ScanProgram::MENU);

        logger->d("onMain",
                  fmt::format("ScanProgramManager is setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));



//        sleep_ms(1000);

//        scanProgramSwapper->swapTo(ScanProgram::MENU);
//        videoSwitch->switchTo(hardware::videoSwitch::PICO);

        logger->d("onMain", "Finished onMain()");
    }

    void ApplicationContainer::onLoop() {
        //dmaManager->onCpu0Loop();
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
//        pi4PowerSwitchManager->setPower(true);
//        sleep_ms(2000);
//        pi4PowerSwitchManager->setPower(false);



        //sleep_ms(2000);
        //heartbeatResponseWriter->sendHeartbeatResponse();


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
    }
} // pico