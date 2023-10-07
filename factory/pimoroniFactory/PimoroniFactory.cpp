//
// Created by stefan on 12/2/22.
//

#include "PimoroniFactory.h"


namespace pico::di {
        void PimoroniFactory::initializeAllSmartPointers() {
            this->logger = std::make_shared<logger::StdioPrintFLogger>();
//            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::MockPi4PowerSwitchManager>(this->logger);

            this->dmaManager = std::make_shared<ibus::dma::DmaManager>(logger, observerRegistry);


            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::GpioPi4PowerSwitchManager>(this->logger);

            this->videoSwitch = std::make_shared<hardware::videoSwitch::max4314::Max4314VideoSwitch>(this->logger);

            std::shared_ptr<config::FlashConfigurationStore> flashConfigurationStore = std::make_shared<config::FlashConfigurationStore>(this->logger);
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


            this->screenPowerManager = std::make_shared<ibus::output::writer::ScreenPowerManager>(
                    defaultConfigurationProvider,
                    logger,
                    dmaManager
            );

            this->softPowerRequestWriter = std::make_shared<ibus::output::writer::SoftPowerRequestWriter>(
                    logger,
                    dmaManager
                    );
            this->mainScreen = std::make_shared<video::ScreenManager::MainScreen::MainScreen>(
                    logger,
                    configurationManager,
                    powerSwitchManager,
                    videoSwitch,
                    configurationStatusWriter,
                    softPowerRequestWriter
                    );
            this->screenManager = std::make_shared<video::ScreenManager::ScreenManager>(
                    std::reinterpret_pointer_cast<video::ScreenManager::Screen>(mainScreen)
                    );


            this->graphicsLib = std::make_shared<video::scanProgram::graphicsLib>();

            this->noopScanProgram = std::make_shared<video::scanProgram::scanPrograms::noop::NoopScanProgram>(logger);
            this->menuScanProgram = std::make_shared<video::scanProgram::scanPrograms::menu::MenuScanProgram>(logger);
            this->demoScanProgram = std::make_shared<video::scanProgram::scanPrograms::demo::DemoScanProgram>(logger);
            this->clockScanProgram = std::make_shared<video::scanProgram::scanPrograms::clock::ClockScanProgram>(logger, graphicsLib);
            this->bootsplashScanProgram = std::make_shared<video::scanProgram::scanPrograms::bootsplash::BootsplashScanProgram>(logger);

            this->scanProgramManager = std::make_shared<video::scanProgram::ScanProgramManager>(
                    logger,
                    noopScanProgram,
                    menuScanProgram,
                    demoScanProgram,
                    clockScanProgram,
                    bootsplashScanProgram
                    );
            this->scanProgramSwapper = std::make_shared<video::scanProgram::ScanProgramSwapper>(
                    logger,
                    scanProgramManager
                    );
            this->mockIncomingIBusObserver = std::make_shared<ibus::observers::MockObserver>(this->logger);

             this->ignitionStateObserver = std::make_shared<ibus::observers::IgnitionStateObserver>(
                     logger,
                     powerSwitchManager,
                     videoSwitch,
                     scanProgramSwapper,
                     screenPowerManager
                     );
             this->knobListenerObserver = std::make_shared<ibus::observers::KnobListenerObserver>(
                     logger,
                     scanProgramSwapper,
                     videoSwitch,
                     screenManager
                     );
             this->heatbeatRequestObserver = std::make_shared<ibus::observers::HeatbeatRequestObserver>(
                     logger,
                     heartbeatResponseWriter
                     );
             this->picoConfigurationReadRequestObserver = std::make_shared<ibus::observers::PicoConfigurationReadRequestObserver>(
                     logger,
                     configurationStatusWriter,
                     configurationManager
                     );
             this->piPowerRequestObserver = std::make_shared<ibus::observers::PiPowerRequestObserver>(
                     logger,
                     powerSwitchManager
                     );
             this->picoVideoRequestObserver = std::make_shared<ibus::observers::PicoVideoRequestObserver>(
                     logger,
                     videoSwitch
                     );
             this->telephoneLongPressObserver = std::make_shared<ibus::observers::TelephoneLongPressObserver>(
                     logger,
                     videoSwitch,
                     scanProgramSwapper,
                     screenPowerManager
                     );

            this->observerRegistry = std::make_shared<ibus::observerRegistry::ObserverRegistry>(this->logger);
            this->baseObservers = std::make_shared<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>>(
                    std::vector<std::shared_ptr<ibus::observers::BaseObserver>>()
                    );

            //Populate the list of observers in the ObserverRegistry and up-cast them.
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(mockIncomingIBusObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(ignitionStateObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(knobListenerObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(heatbeatRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(picoConfigurationReadRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(piPowerRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(picoVideoRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(telephoneLongPressObserver));

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

    } // di