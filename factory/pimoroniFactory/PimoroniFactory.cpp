//
// Created by stefan on 12/2/22.
//

#include "PimoroniFactory.h"


namespace pico {
    namespace di {
        void PimoroniFactory::initializeAllSmartPointers() {
            this->logger = std::make_shared<logger::StdioPrintFLogger>();
//            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::MockPi4PowerSwitchManager>(this->logger);
            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::GpioPi4PowerSwitchManager>(this->logger);
            this->videoSwitch = std::make_shared<hardware::videoSwitch::mock::MockVideoSwitch>(this->logger);
            this->configurationManager = std::make_shared<config::ConfigurationManager>();
            this->scanProgramManager = std::make_shared<video::scanProgram::ScanProgramManager>();
            this->scanProgramSwapper = std::make_shared<video::scanProgram::ScanProgramSwapper>();
            this->mockIncomingIBusObserver = std::make_shared<ibus::observers::MockObserver>(this->logger);
            this->observerRegistry = std::make_shared<ibus::observerRegistry::ObserverRegistry>(this->logger);

            this->baseObservers = std::make_shared<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>>(
                    std::vector<std::shared_ptr<ibus::observers::BaseObserver>>()
                    );

            //Populate the list of observers in the ObserverRegistry and up-cast them.
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(mockIncomingIBusObserver)
            );
        }

        PimoroniFactory::PimoroniFactory() {
            initializeAllSmartPointers();
        }

        ApplicationContainer* PimoroniFactory::getApplicationContainer() {
            return new ApplicationContainer(
                    logger,
                    powerSwitchManager,
                    videoSwitch,
                    configurationManager,
                    scanProgramManager,
                    scanProgramSwapper,
                    observerRegistry,
                    baseObservers
            );
        }


        void PimoroniFactory::deallocateApplicationContainer() {
            //TODO delete everything.
            //TODO then delete the container.
        }

    } // pico
} // di