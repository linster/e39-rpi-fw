//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_BASEOBSERVER_H
#define PICOTEMPLATE_BASEOBSERVER_H

#include <ibus/data/IbusPacket.h>
#include <logging/BaseLogger.h>
#include <proto_cpp/PiToPico.h>
#include <libs/fmt/include/fmt/format.h>

namespace pico::ibus::observers {

            class BaseObserver {

            public:
                void dispatchPacket(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket
                        );

                virtual std::string getTag() = 0;

            protected:
                virtual void onNewPacket(std::shared_ptr<pico::ibus::data::IbusPacket> iBusPacket) = 0;
                messages::PiToPicoMessage decodePiToPicoMessage(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        pico::ibus::data::IbusPacket ibusPacket);

            private:
                /**
                 *  Whether to log "Dispatching Packet" and "Dispatched Packet"
                 *  before the observer does the match. To keep logs from being
                 *  spammy, and observer should only enable this if it is at risk
                 *  of being hung.
                 */
                bool log_packetDispatchTrace = false;

            };

        } // observers

#endif //PICOTEMPLATE_BASEOBSERVER_H
