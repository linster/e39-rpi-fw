//
// Created by stefan on 1/20/24.
//

#ifndef PICOTEMPLATE_STARTUPMANAGER_H
#define PICOTEMPLATE_STARTUPMANAGER_H

#include <memory>
#include "factory/ApplicationContainer.h"
#include "pico/multicore.h"
#include "pico/sync.h"

namespace pico::startupManager {

    class StartupManager {


    private:
        std::shared_ptr<ApplicationContainer> applicationContainer;

        void core1Func();
    public:
        explicit StartupManager(
                std::shared_ptr<ApplicationContainer> applicationContainer
        );

        void cpu0main();


        //https://github.com/raspberrypi/pico-examples/blob/eca13acf57916a0bd5961028314006983894fc84/multicore/multicore_fifo_irqs/multicore_fifo_irqs.c#L53
        // We MUST start the other core before we enabled FIFO interrupts.
        // This is because the launch uses the FIFO's, enabling interrupts before
        // they are used for the launch will result in unexpected behaviour.
        //TODO I think this is why we sometimes crash on boot. We might need to rejig
        //TODO how we start up the cores.


    };

} // startupManager

#endif //PICOTEMPLATE_STARTUPMANAGER_H
