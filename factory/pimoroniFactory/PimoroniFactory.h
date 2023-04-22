//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_PIMORONIFACTORY_H
#define PICOTEMPLATE_PIMORONIFACTORY_H

#include <vector>
#include "../ApplicationContainer.h"
#include "../../logging/StdioPrintFLogger.h"
#include "../../hardware/pi4powerswitch/MockPi4PowerSwitchManager.h"
#include "../../hardware/pi4powerswitch/GpioPi4PowerSwitchManager.h"
#include "../../hardware/videoSwitch/mock/MockVideoSwitch.h"
#include "../../ibus/observerRegistry/observers/BaseObserver.h"
#include "../../ibus/observerRegistry/observers/mockObserver/MockObserver.h"
#include "../../ibus/observerRegistry/ObserverRegistry.h"
#include "../../configuration/default/DefaultConfigurationProvider.h"
#include "../../configuration/default/PimoroniDefaultConfigurationProvider.h"

namespace pico {
    namespace di {

        class PimoroniFactory {

        private:
            std::shared_ptr<logger::BaseLogger> logger;
            std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> powerSwitchManager;
            std::shared_ptr<hardware::videoSwitch::mock::MockVideoSwitch> videoSwitch;
            std::shared_ptr<config::ConfigurationManager> configurationManager;
            std::shared_ptr<video::scanProgram::ScanProgramManager> scanProgramManager;
            std::shared_ptr<video::scanProgram::ScanProgramSwapper> scanProgramSwapper;
            std::shared_ptr<ibus::observers::MockObserver> mockIncomingIBusObserver;
            std::shared_ptr<ibus::observerRegistry::ObserverRegistry> observerRegistry;

            std::shared_ptr<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>> baseObservers;

            std::shared_ptr<ibus::dma::DmaManager> dmaManager;
            std::shared_ptr<ibus::output::writer::HeartbeatResponseWriter> heartbeatResponseWriter;

            void initializeAllSmartPointers();

        public:
            PimoroniFactory();

            ApplicationContainer* getApplicationContainer();

            //Clean up all the memory
            void deallocateApplicationContainer();
        };

    } // pico
} // di

#endif //PICOTEMPLATE_PIMORONIFACTORY_H
