#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include <CMakeHasVideoFlags.h>

#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"

#include "logging/BaseLogger.h"
#include "logging/StdioPrintFLogger.h"


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

    //One-time setup on the second core.
    applicationContainer->onCpu1Main();

    sleep_ms(2000); //Don't start looping until the main cpu0 loop starts.

    while(true) {
        gpio_put(LED_PIN, true);
        applicationContainer->onCpu1Loop();
        gpio_put(LED_PIN, false);
    }
}



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
    multicore_launch_core1(core1_entry); //Launch the coprocessor and have it block.
//    Push the pointer to the application container to the second core. This unblocks the co-processor.
    multicore_fifo_push_blocking((uint32_t)applicationContainer);

    //Wait for the CPU1 setup to complete.
    sleep_ms(3000);
#endif

    while (true) {
        applicationContainer->onLoop();
    }
}
