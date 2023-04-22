//
// Created by stefan on 12/11/22.
//

#ifndef PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H
#define PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H

#include "../../BaseObserver.h"
#include "../../../logging/BaseLogger.h"
#include "../../../../data/IBusDevices.h"
#include "../../../../proto_cpp/PiToPico.h"
#include "../../../../outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h"
#include "../../../configuration/ConfigurationManager.h"

namespace pico {
    namespace ibus {
        namespace observers {

            /**
             * The RPI sends a magic packet asking for our config.
             * we listen for it here, then use it to kick-off a read of the
             * config store, and emit it.
             */
            class PicoConfigurationReadRequestObserver : public BaseObserver{

            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter;
                std::shared_ptr<config::ConfigurationManager> configurationManager;

                void onNewPiToPicoPacket(messages::PiToPicoMessage message);
            public:
                PicoConfigurationReadRequestObserver(
                        std::shared_ptr<logger::BaseLogger> logger,
                        std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                        std::shared_ptr<config::ConfigurationManager> configurationManager
                );

            protected:
                std::string getTag() override { return "PicoConfigurationReadRequestObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_PICOCONFIGURATIONREADREQUESTOBSERVER_H
