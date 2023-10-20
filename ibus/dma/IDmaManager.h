//
// Created by stefan on 10/17/23.
//

#ifndef PICOTEMPLATE_IDMAMANAGER_H
#define PICOTEMPLATE_IDMAMANAGER_H

#include <ibus/data/IbusPacket.h>

namespace pico {
    namespace ibus {
        namespace dma {

            class IDmaManager {

            public:
                //Called only by ApplicationContainer
                virtual void cpu0setup() = 0;
                virtual void onCpu0Loop() = 0;
                virtual void cpu1Setup() = 0;
                virtual void onCpu1Loop() = 0;
                //Called only by ApplicationContainer

                virtual void cpu0scheduleOutgoingMessage(data::IbusPacket packet) = 0; //Called from the base output writer.

                virtual void cpu0scheduleOutgoingProbeOnlyMessage(data::IbusPacket packet) = 0;

                virtual ~IDmaManager() = default;
            };

        } // pico
    } // ibus
} // dma

#endif //PICOTEMPLATE_IDMAMANAGER_H
