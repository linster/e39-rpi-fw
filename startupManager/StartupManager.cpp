//
// Created by stefan on 1/20/24.
//

#include "StartupManager.h"

namespace pico::startupManager {
    StartupManager::StartupManager(ApplicationContainer* applicationContainer) {
        this->applicationContainer = applicationContainer;
    }

    void StartupManager::cpu0main() {

        multicore_reset_core1();

        //https://github.com/raspberrypi/pico-examples/blob/eca13acf57916a0bd5961028314006983894fc84/multicore/multicore_fifo_irqs/multicore_fifo_irqs.c
        //From the above, we need to launch core 1, then execute onMain() to set up interrupts,
        //and then we are able to launch core1.

        // We MUST start the other core before we enabled FIFO interrupts.
        // This is because the launch uses the FIFO's, enabling interrupts before
        // they are used for the launch will result in unexpected behaviour.
        multicore_launch_core1(core1Func);


        applicationContainer->onMain();

        multicore_fifo_push_blocking((uint32_t)applicationContainer);

        uint32_t incomingMessage = multicore_fifo_pop_blocking();

        if (incomingMessage != 1) {
            //TODO somehow log we're out of sequence.
        }

        while (true) {
            tight_loop_contents();
            applicationContainer->onLoop();
        }

    }

    void StartupManager::core1Func() {
        multicore_lockout_victim_init();

//        if (!multicore_lockout_victim_is_initialized(1)) { // Might need an SDK update to get this
//            TODO somehow log bad things
//        }

        ApplicationContainer* applicationContainerPointer = (ApplicationContainer*)multicore_fifo_pop_blocking();
        applicationContainerPointer->onCpu1Main();

        multicore_fifo_push_blocking(1);

        while(true) {
            tight_loop_contents();
            gpio_put(25, true);
            applicationContainer->onCpu1Loop();
            gpio_put(25, false);
        }

    }
} // startupManager