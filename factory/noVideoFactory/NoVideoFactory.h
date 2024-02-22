//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_NOVIDEO_FACTORY_H
#define PICOTEMPLATE_NOVIDEO_FACTORY_H

#include <vector>
#include <factory/ApplicationContainerNoVideo.h>

#include <logging/StdioPrintFLogger.h>
#include <logging/factory/LoggerFactory.h>
#include <hardware/pi4powerswitch/MockPi4PowerSwitchManager.h>
#include <hardware/pi4powerswitch/GpioPi4PowerSwitchManager.h>
#include <hardware/videoSwitch/mock/MockVideoSwitch.h>
#include <hardware/videoSwitch/max4314/Max4314VideoSwitch.h>

#include <ibus/dma/SingleCoreDmaManager.h>

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <ibus/observerRegistry/observers/mockObserver/MockObserver.h>
#include <ibus/observerRegistry/observers/ignitionStateNoVideo/IgnitionStateNoVideoObserver.h>
#include <ibus/observerRegistry/observers/knobListener/KnobListenerObserver.h>
#include <ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.h>
#include <ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h>
#include <ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.h>
#include <ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.h>
#include <ibus/observerRegistry/observers/telephonePressNoVideo/TelephonePressNoVideo.h>

#include <ibus/outputWriterApi/TestingOutputWriter.h>
#include <ibus/outputWriterApi/radioControl/AudioFocusOutputWriter.h>

#include <ibus/observerRegistry/ObserverRegistry.h>
#include <configuration/default/DefaultConfigurationProvider.h>
#include <configuration/default/PimoroniDefaultConfigurationProvider.h>

#include "ibus/outputWriterApi/uartForwarderWriter/UartForwarderWriter.h"
#include "ibus/observerRegistry/observers/uartForwarderObserver/UartForwarderObserver.h"
#include "ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h"

namespace pico::di {

        class NoVideoFactory {

        private:

            std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager;
            std::shared_ptr<logger::factory::LoggerFactory> loggerFactory;

            std::shared_ptr<logger::BaseLogger> logger;
            std::shared_ptr<watchdog::WatchdogManager> watchdogManager;

            std::shared_ptr<hardware::pi4powerswitch::IPi4PowerSwitchManager> powerSwitchManager;
            std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;

            std::shared_ptr<config::ConfigurationManager> configurationManager;

            std::shared_ptr<ibus::output::writer::ScreenPowerManager> screenPowerManager;
            std::shared_ptr<ibus::output::writer::SoftPowerRequestWriter> softPowerRequestWriter;

            std::shared_ptr<pico::ibus::output::writer::TestingOutputWriter> testingOutputWriter;
            std::shared_ptr<pico::ibus::output::writer::radio::AudioFocusOutputWriter> audioFocusOutputWriter;
            std::shared_ptr<pico::ibus::output::writer::UartForwarderWriter> uartForwarderWriter;



            std::shared_ptr<ibus::observers::MockObserver> mockIncomingIBusObserver;

            std::shared_ptr<ibus::observers::IgnitionStateNoVideoObserver> ignitionStateNoVideoObserver;

            std::shared_ptr<ibus::observers::HeatbeatRequestObserver> heatbeatRequestObserver;
            std::shared_ptr<ibus::observers::PicoConfigurationReadRequestObserver> picoConfigurationReadRequestObserver;
            std::shared_ptr<ibus::observers::PiPowerRequestObserver> piPowerRequestObserver;
            std::shared_ptr<ibus::observers::PicoVideoRequestObserver> picoVideoRequestObserver;
            std::shared_ptr<ibus::observers::TelephonePressNoVideo> telephonePressNoVideoObserver;
            std::shared_ptr<ibus::observers::UartForwarderObserver> uartForwarderObserver;

            std::shared_ptr<ibus::observerRegistry::ObserverRegistry> observerRegistry;

            std::shared_ptr<std::vector<std::shared_ptr<ibus::observers::BaseObserver>>> baseObservers;

            std::shared_ptr<ibus::dma::IDmaManager> dmaManager;
            std::shared_ptr<ibus::output::writer::HeartbeatResponseWriter> heartbeatResponseWriter;

            void initializeAllSmartPointers();

        public:
            NoVideoFactory();

            ApplicationContainerNoVideo* getApplicationContainer();

            //Clean up all the memory
            void deallocateApplicationContainer();
        };

    } // di

#endif //PICOTEMPLATE_NOVIDEO_FACTORY_H
