#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include <CMakeHasVideoFlags.h>

#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"

#include "logging/BaseLogger.h"
#include "logging/StdioPrintFLogger.h"
#include "logging/Core1DeferringLogger.h"


#ifdef CMAKE_HAS_VIDEO_SUPPORT
#include "factory/pimoroniFactory/PimoroniFactory.h"
#else
#include "factory/noVideoFactory/NoVideoFactory.h"
#endif



// The built in LED
#define LED_PIN 25
#define TEMP_ADC 4


void core1_entry() {
    //The very first thing we get on startup wait and block for a pointer to the application container.
    #ifdef CMAKE_HAS_VIDEO_SUPPORT
        auto* applicationContainer = (pico::ApplicationContainer*) multicore_fifo_pop_blocking();
    #else
    auto* applicationContainer = (pico::ApplicationContainerNoVideo*) multicore_fifo_pop_blocking();
    #endif

    //Let core 0 park this core in RAM while it erases/programs flash (FlashConfigurationStore).
    //Must come after the FIFO pop above: the lockout handler takes over the inter-core FIFO IRQ
    //and would swallow the application container pointer.
    multicore_lockout_victim_init();

    //One-time setup on the second core.
    applicationContainer->onCpu1Main();

    sleep_ms(2000); //Don't start looping until the main cpu0 loop starts.

    while(true) {
        gpio_put(LED_PIN, true);
        applicationContainer->onCpu1Loop();
        gpio_put(LED_PIN, false);
    }
}



#ifdef CMAKE_HAS_VIDEO_SUPPORT
//Core 1's stack (see multicore_launch_core1_with_stack in main). Peak measured use ~1.8 KB.
static uint32_t core1Stack[8 * 1024 / sizeof(uint32_t)];
#endif

int main() {

    //stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
//
//    adc_init();
//    adc_gpio_init(26);
//    adc_set_temp_sensor_enabled(true);
//    adc_select_input(TEMP_ADC);

    #ifdef CMAKE_HAS_VIDEO_SUPPORT
        auto* factory = new pico::di::PimoroniFactory();
        pico::ApplicationContainer* applicationContainer = factory->getApplicationContainer();
    #else
        auto* factory = new pico::di::NoVideoFactory();
        pico::ApplicationContainerNoVideo* applicationContainer = factory->getApplicationContainer();
    #endif

    applicationContainer->onMain(); //Run main one-time setup code.

#ifdef CMAKE_HAS_VIDEO_SUPPORT
    multicore_reset_core1();
    //Give core 1 its own stack in main RAM instead of the SDK default (the top of SCRATCH_X).
    //Core 0's stack is SCRATCH_Y (4 KB) and grows down into SCRATCH_X, so with the default layout a
    //deep core 0 call chain (e.g. IBus knob input -> menu refreshUi -> drawScreen -> computeFrame)
    //ran past 4 KB and overwrote core 1's live stack frames; core 1 then "returned" into core 0's
    //stack and HardFaulted. Measured on hardware: core 0 reached the bottom of SCRATCH_Y during
    //menu navigation. With core 1's stack elsewhere, core 0 has SCRATCH_Y + SCRATCH_X (8 KB).
    multicore_launch_core1_with_stack(core1_entry, core1Stack, sizeof(core1Stack)); //Launch the coprocessor and have it block.
//    Push the pointer to the application container to the second core. This unblocks the co-processor.
    multicore_fifo_push_blocking((uint32_t)applicationContainer);

    //Wait for the CPU1 setup to complete.
    sleep_ms(3000);
#endif

    while (true) {
        applicationContainer->onLoop();
        pico::logger::Core1DeferringLogger::drainOnCore0();
    }
}
