//
// Created by stefan on 2/22/24.
//

#ifndef PICOTEMPLATE_PICOCONFIGPUSHOBSERVER_H
#define PICOTEMPLATE_PICOCONFIGPUSHOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <logging/BaseLogger.h>
#include <ibus/data/IBusDevices.h>
#include <proto_cpp/PiToPico.h>
#include <ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h>
#include <configuration/ConfigurationManager.h>

namespace pico::ibus::observers {

    //TODO put this in both factories and both observer lists.

    class PicoConfigPushObserver : public BaseObserver {

    public:
        PicoConfigPushObserver(
                std::shared_ptr<logger::BaseLogger> logger,
                std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter,
                std::shared_ptr<config::ConfigurationManager> configurationManager
        );
    private:
        std::shared_ptr<logger::BaseLogger> logger;
        std::shared_ptr<output::writer::ConfigurationStatusWriter> configurationStatusWriter;
        std::shared_ptr<config::ConfigurationManager> configurationManager;

        void onNewPiToPicoPacket(messages::PiToPicoMessage message);

    protected:
        std::string getTag() override { return "PicoConfigPushObserver"; };
        void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) override;
    };

} // observers

#endif //PICOTEMPLATE_PICOCONFIGPUSHOBSERVER_H
