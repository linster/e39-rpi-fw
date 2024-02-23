//
// Created by stefan on 12/2/22.
//

#include "NoVideoFactory.h"
#include "ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"


namespace pico::di {
        void NoVideoFactory::initializeAllSmartPointers() {
            //TODO we have a knot. The logger factory needs dma manager to build a dependency,
            //TODO but dma manager needs a logger.

            this->busTopologyManager = std::make_shared<pico::ibus::topology::BusTopologyManager>();


            //TODO we might want to make this a class variable so it isn't popped off the stack when
            //TODO this method completes.
            std::function<std::shared_ptr<pico::ibus::dma::IDmaManager>()> dmaManagerAccessor = [this](){
                return this->dmaManager;
            };

            this->loggerFactory = std::make_shared<logger::factory::LoggerFactory>(dmaManagerAccessor, busTopologyManager);

            this->logger = loggerFactory->buildAndGetLogger();

            this->observerRegistry = std::make_shared<ibus::observerRegistry::ObserverRegistry>(this->logger);
            this->dmaManager = std::make_shared<ibus::dma::SingleCoreDmaManager>(logger, observerRegistry, busTopologyManager);


            this->watchdogManager = std::make_shared<watchdog::WatchdogManager>(logger);

            //this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::MockPi4PowerSwitchManager>(this->logger);
            this->powerSwitchManager = std::make_shared<hardware::pi4powerswitch::GpioPi4PowerSwitchManager>(this->logger);

            this->videoSwitch = std::make_shared<hardware::videoSwitch::max4314::Max4314VideoSwitch>(this->logger);

            this->baseObservers = std::make_shared<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>>(
                    std::vector<std::shared_ptr<ibus::observers::BaseObserver>>()
            );

            //TODO we might want to make this a class variable so it isn't popped off the stack when
            //TODO this method completes.
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

            //All the outputWriters need to be initialized before any observers that depend on them.
            this->heartbeatResponseWriter = std::make_shared<ibus::output::writer::HeartbeatResponseWriter>(
                    logger,
                    dmaManager
            );

            this->screenPowerManager = std::make_shared<ibus::output::writer::ScreenPowerManager>(
                    defaultConfigurationProvider,
                    logger,
                    dmaManager
            );

            this->softPowerRequestWriter = std::make_shared<ibus::output::writer::SoftPowerRequestWriter>(
                    logger,
                    dmaManager
            );

            this->uartForwarderWriter = std::make_shared<pico::ibus::output::writer::UartForwarderWriter>(
                    this->logger,
                    this->dmaManager
                    );

            this->mockIncomingIBusObserver = std::make_shared<ibus::observers::MockObserver>(
                    this->logger,
                    this->busTopologyManager
                    );

             this->ignitionStateNoVideoObserver = std::make_shared<ibus::observers::IgnitionStateNoVideoObserver>(
                     logger,
                     powerSwitchManager,
                     videoSwitch,
                     screenPowerManager
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
             this->picoConfigPushObserver = std::make_shared<ibus::observers::PicoConfigPushObserver>(
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
             this->audioFocusOutputWriter = std::make_shared<pico::ibus::output::writer::radio::AudioFocusOutputWriter>(
                     logger,
                     dmaManager
                     );
             this->testingOutputWriter = std::make_shared<pico::ibus::output::writer::TestingOutputWriter>(
                     logger,
                     dmaManager,
                     audioFocusOutputWriter
                     );
             this->telephonePressNoVideoObserver = std::make_shared<ibus::observers::TelephonePressNoVideo>(
                     logger,
                     videoSwitch,
                     screenPowerManager,
                     testingOutputWriter,
                     powerSwitchManager
                     );
             this->uartForwarderObserver = std::make_shared<ibus::observers::UartForwarderObserver>(
                     this->logger,
                     this->busTopologyManager,
                     this->uartForwarderWriter,
                     this->videoSwitch
                     );


            //Populate the list of observers in the ObserverRegistry and up-cast them.
            //We have an observer registry that is populated here because we need to avoid the circular dependency
            //on dmaManager. This is poor-man's Dagger Provider<T>.
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(mockIncomingIBusObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(ignitionStateNoVideoObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(heatbeatRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(picoConfigurationReadRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(picoConfigPushObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(piPowerRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(picoVideoRequestObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(telephonePressNoVideoObserver));
            baseObservers->push_back(
                    std::static_pointer_cast<ibus::observers::BaseObserver>(uartForwarderObserver));
        }

        NoVideoFactory::NoVideoFactory() {
            initializeAllSmartPointers();
        }

        ApplicationContainerNoVideo* NoVideoFactory::getApplicationContainer() {
            return new ApplicationContainerNoVideo(
                    logger,
                    busTopologyManager,
                    watchdogManager,
                    powerSwitchManager,
                    videoSwitch,
                    configurationManager,
                    observerRegistry,
                    baseObservers,
                    dmaManager,
                    heartbeatResponseWriter,
                    audioFocusOutputWriter
            );
        }


        void NoVideoFactory::deallocateApplicationContainer() {
            //TODO delete everything.
            //TODO then delete the container.
        }

    } // di