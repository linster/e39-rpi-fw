//
// Created by stefan on 1/2/24.
//

#ifndef PICOTEMPLATE_UARTFORWARDERWRITER_H
#define PICOTEMPLATE_UARTFORWARDERWRITER_H

#include <ibus/outputWriterApi/BaseOutputWriter.h>
#include <logging/BaseLogger.h>
#include "ibus/dma/topology/BusTopologyManager.h"


namespace pico::ibus::output::writer {

    class UartForwarderWriter : public BaseOutputWriter {

    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<dma::IDmaManager> dmaManager;

    public:
        UartForwarderWriter(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<dma::IDmaManager> dmaManager
                );

        void forwardPacketToCar(data::IbusPacket packet);
        void forwardPacketToPi(data::IbusPacket packet);
    protected:
        std::string getTag() override { return "UartForwarderWriter"; };
        std::shared_ptr<dma::IDmaManager> getDmaManager() override;
    };

} // writer

#endif //PICOTEMPLATE_UARTFORWARDERWRITER_H
