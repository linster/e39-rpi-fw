//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_APPLICATIONCONTAINER_H
#define PICOTEMPLATE_APPLICATIONCONTAINER_H

#include <memory>
#include "../logging/BaseLogger.h"
#include "../hardware/pi4powerswitch/IPi4PowerSwitchManager.h"
#include "../hardware/videoSwitch/VideoSwitch.h"
#include "../configuration/ConfigurationManager.h"
#include "../video/scanProgram/ScanProgramManager.h"
#include "../video/scanProgram/ScanProgramSwapper.h"
#include "../ibus/observerRegistry/ObserverRegistry.h"

namespace pico {

    class ApplicationContainer {

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager;
        std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch;
        std::shared_ptr<config::ConfigurationManager> configurationManager;
        std::shared_ptr<video::scanProgram::ScanProgramManager> scanProgramManager;
        std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
        std::shared_ptr<ibus::observerRegistry::ObserverRegistry> observerRegistry;
        std::shared_ptr<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>> baseObservers;


        //Setup the second core.
        void setupCpu1();
    public:

        ApplicationContainer(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> pi4PowerSwitchManager,
                std::shared_ptr<hardware::videoSwitch::VideoSwitch> videoSwitch,
                std::shared_ptr<config::ConfigurationManager> configurationManager,
                std::shared_ptr<video::scanProgram::ScanProgramManager> scanProgramManager,
                std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper,
                std::shared_ptr<ibus::observerRegistry::ObserverRegistry> observerRegistry,
                std::shared_ptr<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>> baseObservers
                ) {
            this->logger = logger;
            this->pi4PowerSwitchManager = pi4PowerSwitchManager;
            this->videoSwitch = videoSwitch;
            this->configurationManager = configurationManager;
            this->scanProgramManager = scanProgramManager;
            this->scanProgramSwapper = scanProgramSwapper;
            this->observerRegistry = observerRegistry;
            this->baseObservers = baseObservers;
        };

        //Run once on main CPU main()
        void onMain();
        //Run once on the main CPU loop()
        void onLoop();

        //Run once on the co-processor
        void onCpu1Main();
        //Run once on co-processor loop
        void onCpu1Loop();
    };

} // pico

#endif //PICOTEMPLATE_APPLICATIONCONTAINER_H
