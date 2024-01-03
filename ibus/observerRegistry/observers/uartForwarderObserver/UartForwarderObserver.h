//
// Created by stefan on 1/2/24.
//

#ifndef PICOTEMPLATE_UARTFORWARDEROBSERVER_H
#define PICOTEMPLATE_UARTFORWARDEROBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <logging/BaseLogger.h>
#include "ibus/dma/topology/BusTopologyManager.h"
#include "ibus/outputWriterApi/uartForwarderWriter/UartForwarderWriter.h"
#include "hardware/videoSwitch/VideoSwitch.h"

namespace pico::ibus::observers {

    class UartForwarderObserver : public BaseObserver {

    public:
        UartForwarderObserver(
                std::shared_ptr<pico::logger::BaseLogger> logger,
                std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager,
                std::shared_ptr<pico::ibus::output::writer::UartForwarderWriter> writer,
                std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch
        );

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<pico::ibus::topology::BusTopologyManager> busTopologyManager;
        std::shared_ptr<pico::ibus::output::writer::UartForwarderWriter> writer;
        std::shared_ptr<pico::hardware::videoSwitch::VideoSwitch> videoSwitch;

        bool shouldForwardPacketToCar(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket);
    protected:
        std::string getTag() override { return "UartForwarderObserver"; };
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;

    };

} // observers

#endif //PICOTEMPLATE_UARTFORWARDEROBSERVER_H
