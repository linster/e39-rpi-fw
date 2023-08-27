//
// Created by stefan on 12/2/22.
//

#include "PimoroniFactory.h"


namespace pico {
    namespace di {
        void PimoroniFactory::initializeAllSmartPointers() {
            this->logger = std::make_shared<logger::StdioPrintFLogger>();
//            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::MockPi4PowerSwitchManager>(this->logger);

            this->dmaManager = std::make_shared<ibus::dma::DmaManager>(logger, observerRegistry);


            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::GpioPi4PowerSwitchManager>(this->logger);

            this->videoSwitch = std::make_shared<hardware::videoSwitch::max4314::Max4314VideoSwitch>(this->logger);

            std::shared_ptr<config::FlashConfigurationStore> flashConfigurationStore = std::make_shared<config::FlashConfigurationStore>();
            std::shared_ptr<ibus::output::writer::ConfigurationStatusWriter> configurationStatusWriter =
                    std::make_shared<ibus::output::writer::ConfigurationStatusWriter>(
                            logger, dmaManager);
            std::shared_ptr<config::IBusConfigMessageStore> iBusConfigMessageStore =
                    std::make_shared<config::IBusConfigMessageStore>(
                            logger,
                            configurationStatusWriter);
            std::shared_ptr<config::MemoryConfigurationStore> memoryConfigurationStore =
                    std::make_shared<config::MemoryConfigurationStore>();
            std::shared_ptr<config::defaults::PimoroniDefaultConfigurationProvider> defaultConfigurationProvider =
                    std::make_shared<config::defaults::PimoroniDefaultConfigurationProvider>();

            this->configurationManager = std::make_shared<config::ConfigurationManager>(
                    flashConfigurationStore,
                    iBusConfigMessageStore,
                    memoryConfigurationStore,
                    logger,
                    defaultConfigurationProvider);


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

            this->heartbeatResponseWriter = std::make_shared<ibus::output::writer::HeartbeatResponseWriter>(
                    logger,
                    dmaManager
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
                    baseObservers,
                    dmaManager,
                    heartbeatResponseWriter
            );
        }


        void PimoroniFactory::deallocateApplicationContainer() {
            //TODO delete everything.
            //TODO then delete the container.
        }

    } // pico
} // di