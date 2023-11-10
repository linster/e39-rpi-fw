//
// Created by stefan on 12/2/22.
//

#ifndef PICOTEMPLATE_OBSERVERREGISTRY_H
#define PICOTEMPLATE_OBSERVERREGISTRY_H

#include <vector>
#include <ibus/observerRegistry/observers/BaseObserver.h>
#include <ibus/data/IbusPacket.h>
namespace pico {
    namespace ibus {
        namespace observerRegistry {

            class ObserverRegistry {


            private:
                std::shared_ptr<logger::BaseLogger> logger;
                std::vector<std::shared_ptr<pico::ibus::observers::BaseObserver>> observerList;

            public:
                ObserverRegistry(std::shared_ptr<pico::logger::BaseLogger> logger);
                void registerObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer);
                void unregisterObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer);

                void dispatchMessageToAllObservers(data::IbusPacket packet);

                void printRegisteredObserverTags();
            };
        } // pico
    } // ibus
} // observerRegistry

#endif //PICOTEMPLATE_OBSERVERREGISTRY_H
