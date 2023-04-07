//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_BASEOBSERVER_H
#define PICOTEMPLATE_BASEOBSERVER_H

#include "../../data/IbusPacket.h"
#include "../../../logging/BaseLogger.h"
#include "../../../proto_cpp/PiToPico.h"

namespace pico {
    namespace ibus {
        namespace observers {

            class BaseObserver {

            public:
                void dispatchPacket(
                        std::shared_ptr<pico::logger::BaseLogger> logger,
                        pico::ibus::data::IbusPacket iBusPacket
                        );

                virtual std::string getTag() = 0;

            protected:
                virtual void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) = 0;
                messages::PiToPicoMessage decodePiToPicoMessage(pico::ibus::data::IbusPacket ibusPacket);
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_BASEOBSERVER_H
