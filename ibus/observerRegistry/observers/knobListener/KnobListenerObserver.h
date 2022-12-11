//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_KNOBLISTENEROBSERVER_H
#define PICOTEMPLATE_KNOBLISTENEROBSERVER_H

#include <memory>
#include "../../../../logging/BaseLogger.h"
#include "../BaseObserver.h"

namespace pico {
    namespace ibus {
        namespace observers {

            class KnobListenerObserver : public BaseObserver{

            public:
                KnobListenerObserver(
                        std::shared_ptr<logger::BaseLogger> baseLogger
                );
            private:
                std::shared_ptr<logger::BaseLogger> logger;
                void onKnobTurnedRight(int clicks);
                void onKnobTurnedLeft(int clicks);
                void onKnobPressed();
            protected:
                std::string getTag() override { return "KnobListenerObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_KNOBLISTENEROBSERVER_H
