//
// Created by stefan on 12/2/22.
//

#include "ApplicationContainer.h"
#include <libs/fmt/include/fmt/format.h>
#include "pico/time.h"

namespace pico {
    void ApplicationContainer::onMain() {

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

        //Load the stored configuration now, or write the defaults on first boot. Until now nothing
        //read the configuration at startup, so it was never saved to flash on its own and a stored
        //one was only loaded when something (a Pi config request, the menu) asked for it.
        //Doing it here, on core 0 before core 1 starts video, means a first-boot flash write only
        //needs interrupts off and doesn't have to park core 1 mid-frame.
        config::Configuration* configuration = configurationManager->getMutableConfiguration();
        logger->i("onMain", fmt::format("Configuration loaded (rpiFwGitCommitHash '{}')",
                                        configuration->toMessage().rpiFwGitCommitHash));

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
        //The video source and scan program to show at boot come from the configuration
        //(videoSourceOnBoot, scanProgramOnBoot). These used to be hard-coded to PICO and the
        //Linster OS bootsplash; the Pimoroni default configuration keeps that.
        hardware::videoSwitch::VideoSource videoSourceOnBoot = configuration->getVideoSourceOnBootup();
        messages::ConfigMessage::ScanProgram scanProgramOnBoot = configuration->getScanProgramOnBootup();
        logger->i("onMain", fmt::format("Boot video source {}, scan program {} (from configuration)",
                                        (int) videoSourceOnBoot, (int) scanProgramOnBoot));
        videoSwitch->switchTo(videoSourceOnBoot);

        logger->d("onMain",
                  fmt::format("ScanProgramManager not setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));

        scanProgramManager->cpu0setup();
        scanProgramManager->swapToBootProgram(scanProgramOnBoot);

        logger->d("onMain",
                  fmt::format("ScanProgramManager is setup. Current Scan Program is {:x} ",
                              (int)scanProgramManager->getCurrentScanProgram()));

        logger->d("onMain", "Finished onMain()");

    }

    void ApplicationContainer::onLoop() {
        watchdogManager->onCpu0Loop();
        dmaManager->onCpu0Loop();
        scanProgramManager->onCpu0Loop();
    }

    void ApplicationContainer::onCpu1Main() {
        logger->i("onCpu1Main", "onCpu1Main");
        watchdogManager->cpu1Setup();
        dmaManager->cpu1Setup(); //NOOP with SingleCoreDmaManager
        logger->i("onCpu1Main", "dmaManager->cpu1Setup() done");

        logger->i("onCpu1Main", "scanProgramManager->cpu1setup()");
        scanProgramManager->cpu1setup();
        logger->i("onCpu1Main", "scanProgramManager->cpu1setup() done");
    }

    void ApplicationContainer::onCpu1Loop() {
        watchdogManager->onCpu1Loop();
        dmaManager->onCpu1Loop(); //NOOP with SingleCoreDmaManager
        scanProgramManager->onCpu1Loop();
    }
} // pico