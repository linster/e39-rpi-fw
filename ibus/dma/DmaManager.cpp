//
// Created by stefan on 4/7/23.
//

#include "DmaManager.h"

namespace pico {
    namespace ibus {
        namespace dma {

            DmaManager::DmaManager(
                    std::shared_ptr<logger::BaseLogger> logger,
                    std::shared_ptr<observerRegistry::ObserverRegistry> observerRegistry
                    ) {
                this->logger = logger;
                this->observerRegistry = observerRegistry;

                queue_init_with_spinlock(
                        &this->incomingQ,
                        255, //Assume each packet is 255 bytes long. (Max_length for the rawBytes)
                        4,
                        1
                );

                queue_init_with_spinlock(
                        &this->outgoingQ,
                        255,
                        4,
                        2
                );
            }

            void DmaManager::setupFromCarPioProgram() {

            }

            void DmaManager::setupToCarPioProgram() {

            }

            void DmaManager::onCpu0Loop() {
                //We check the incoming queue without blocking on empty. If there's something in there,
                //We dispatch it to the observer registry.

                void* incomingPacketBuffer = std::calloc(255, 1);
                bool havePacket = queue_try_remove(&incomingQ, incomingPacketBuffer);
                if (havePacket) {
                    logger->d("DmaManager" ,"onCpu0Loop have packet");
                    //Heap-allocate a new packet from the buffer.
                    std::unique_ptr<data::IbusPacket> newPacket = std::make_unique<data::IbusPacket>(new data::IbusPacket(incomingPacketBuffer));
                    onCpu0IncomingPacket(std::move(newPacket));
                    free(incomingPacketBuffer);
                }
            }

            void DmaManager::onCpu1Loop() {
                //We check the outgoing queue without blocking on empty. If there's something,
                //Try pushing it into pico_sm_put_blocking into the ToCarProgram.pio.
            }

            void DmaManager::cpu0scheduleOutgoingMessage(data::IbusPacket packet) {
                //Do a blocking write into the outgoing queue.
                //The outgoing queue will feed the ToCar and FromCar programs.
                //There won't be a duplicated event, because the interrupt service routine
                //TODO we need to pad the pointers returned by data() so that we're always
                //TODO inserting 255-byte blocks, because queue_add_blocking will always
                //TODO use the underlying Q's [size] parameter.
                //TODO otherwise, we'll overrun the pointer
                queue_add_blocking(&outgoingQ, (void*) packet.getRawPacket().data());
            }

            void DmaManager::onCpu1IncomingPacket(std::unique_ptr<data::IbusPacket> packet) {
                //An incoming packet came in from an interrupt service routine (called by DMA)
                //We need to put this into the incomingQ.
                //TODO we need to pad the pointers returned by data() so that we're always
                //TODO inserting 255-byte blocks, because queue_add_blocking will always
                //TODO use the underlying Q's [size] parameter.
                //TODO otherwise, we'll overrun the pointer
                queue_add_blocking(&incomingQ, (void*)packet->getRawPacket().data());
            }

            void DmaManager::onCpu0IncomingPacket(std::unique_ptr<data::IbusPacket> packet) {
                observerRegistry->dispatchMessageToAllObservers(*packet);
            }
        } // pico
    } // ibus
} // dma