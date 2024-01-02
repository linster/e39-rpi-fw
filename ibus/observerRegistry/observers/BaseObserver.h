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

                //TODO we need a way for an observer to say whether the packet should
                //TODO be forwarded. We might also need a way to specify from which Uart the packet
                //TODO originated from?

                //TODO we might actually want a box at a higher level than the observer registry. Because,
                //TODO observers registered in the registry are for local logic..... but each local logic should
                //TODO know from which uart the packet came from. So just add a flag to the packet and then we're ok.
                //TODO then no need to worry about another box, or filtering packets beyond the plan of having knob
                //TODO listenining s

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
