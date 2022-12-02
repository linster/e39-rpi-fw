//
// Created by stefan on 12/2/22.
//

#include "PimoroniFactory.h"

namespace pico {
    namespace di {
        void PimoroniFactory::initializeAllSmartPointers() {
            logger = std::make_shared<logger::StdioPrintFLogger>();
            powerSwitchManager = std::make_shared<hardware::pi4powerswitch::MockPi4PowerSwitchManager>(logger);
            videoSwitch = std::make_shared<hardware::videoSwitch::mock::MockVideoSwitch>(logger);
            configurationManager = std::make_shared<config::ConfigurationManager>();
            scanProgramManager = std::make_shared<video::scanProgram::ScanProgramManager>();
            scanProgramSwapper = std::make_shared<video::scanProgram::ScanProgramSwapper>();
            mockIncomingIBusObserver = std::make_shared<ibus::observers::MockObserver>(logger);
            observerRegistry = std::make_shared<ibus::observerRegistry::ObserverRegistry>(logger);

            baseObservers = std::make_shared<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>>(
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

        ApplicationContainer PimoroniFactory::getApplicationContainer() {
            return {
                    logger,
                    powerSwitchManager,
                    videoSwitch,
                    configurationManager,
                    scanProgramManager,
                    scanProgramSwapper,
                    observerRegistry,
                    baseObservers
            };
        }


        void PimoroniFactory::deallocateApplicationContainer() {
            //TODO delete everything.
        }

    } // pico
} // di