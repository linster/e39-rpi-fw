//
// Created by stefan on 12/2/22.
//

#include "ObserverRegistry.h"
#include <algorithm>
namespace pico::ibus::observerRegistry {
            ObserverRegistry::ObserverRegistry(std::shared_ptr<pico::logger::BaseLogger> logger) {
                this->logger = logger;
                this->logger->d("ObserverRegistry", "Construction");
            }

            void ObserverRegistry::registerObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer) {
                logger->d("ObserverRegistry", "registerObserver");
                this->observerList.push_back(observer);
            }

            void ObserverRegistry::unregisterObserver(std::shared_ptr<pico::ibus::observers::BaseObserver> observer) {
                auto findResult = std::find(observerList.begin(), observerList.end(), observer);
                if (findResult != observerList.end()) {
                    //We found it, because findResult == __last if not found
                    observerList.erase(findResult);
                }
            }

            void ObserverRegistry::dispatchMessageToAllObservers(data::IbusPacket packet) {
                std::shared_ptr packetPtr = std::make_shared<data::IbusPacket>(packet);
                for (const auto &item: observerList) {
                    item->dispatchPacket(logger, packetPtr);
                }
            }

            void ObserverRegistry::printRegisteredObserverTags() {
                for (const auto &item: observerList) {
                    logger->d(
                            "Observer registry",
                            fmt::format("Registered observer: {}", item->getTag())
                            );
                }
            }
        } // observerRegistry