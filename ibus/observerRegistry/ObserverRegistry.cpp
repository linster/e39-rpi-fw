//
// Created by stefan on 12/2/22.
//

#include "ObserverRegistry.h"

namespace pico {
    namespace ibus {
        namespace observerRegistry {
            ObserverRegistry::ObserverRegistry(std::shared_ptr<pico::logger::BaseLogger> logger) {
                this-> logger = logger;
                this->logger->d("ObserverRegistry", "Construction");
            }

            void ObserverRegistry::registerObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer) {

            }

            void ObserverRegistry::unregisterObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer) {

            }
        } // pico
    } // ibus
} // observerRegistry