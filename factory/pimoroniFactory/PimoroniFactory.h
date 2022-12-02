//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_PIMORONIFACTORY_H
#define PICOTEMPLATE_PIMORONIFACTORY_H

#include <vector>
#include "../ApplicationContainer.h"
#include "../../logging/StdioPrintFLogger.h"
#include "../../hardware/pi4powerswitch/MockPi4PowerSwitchManager.h"
#include "../../hardware/videoSwitch/mock/MockVideoSwitch.h"
#include "../../ibus/observerRegistry/observers/BaseObserver.h"
#include "../../ibus/observerRegistry/observers/mockObserver/MockObserver.h"
#include "../../ibus/observerRegistry/ObserverRegistry.h"

namespace pico {
    namespace di {

        class PimoroniFactory {

        private:
            static std::shared_ptr<logger::StdioPrintFLogger> logger;
            static std::shared_ptr<hardware::pi4powerswitch::MockPi4PowerSwitchManager> powerSwitchManager;
            static std::shared_ptr<hardware::videoSwitch::mock::MockVideoSwitch> videoSwitch;
            static std::shared_ptr<config::ConfigurationManager> configurationManager;
            static std::shared_ptr<video::scanProgram::ScanProgramManager> scanProgramManager;
            static std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
            static std::shared_ptr<ibus::observers::MockObserver> mockIncomingIBusObserver;
            static std::shared_ptr<ibus::observerRegistry::ObserverRegistry> observerRegistry;

            static std::shared_ptr<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>> baseObservers;

            void initializeAllSmartPointers();

        public:
            PimoroniFactory();

            static ApplicationContainer getApplicationContainer();

            //Clean up all the memory
            void deallocateApplicationContainer();
        };

    } // pico
} // di

#endif //PICOTEMPLATE_PIMORONIFACTORY_H
