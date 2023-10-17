//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_MOCKOBSERVER_H
#define PICOTEMPLATE_MOCKOBSERVER_H

#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <logging/BaseLogger.h>
namespace pico {
    namespace ibus {
        namespace observers {

            class MockObserver : public BaseObserver {

            public:
                MockObserver(
                        std::shared_ptr<pico::logger::BaseLogger> logger
                        );

            private:
                std::shared_ptr<logger::BaseLogger> logger;

            protected:
                std::string getTag() override { return "MockObserver"; };
                void onNewPacket(pico::ibus::data::IbusPacket iBusPacket) override;
            };

        } // pico
    } // ibus
} // observers

#endif //PICOTEMPLATE_MOCKOBSERVER_H
