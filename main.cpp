#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/scanvideo.h"
#include "pico/scanvideo/composable_scanline.h"
#include "pico/multicore.h"
#include "pico/sync.h"
#include "pico/stdlib.h"
#include "VGADemo.h"
#include "logging/BaseLogger.h"
#include "logging/StdioPrintFLogger.h"
#include "factory/pimoroniFactory/PimoroniFactory.h"

// The built in LED
#define LED_PIN 25
#define TEMP_ADC 4


void core1_entry() {
    //The very first thing we get on startup wait and block for a pointer to the application container.
    auto* applicationContainer = (pico::ApplicationContainer*) multicore_fifo_pop_blocking();

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

    auto* factory = new pico::di::PimoroniFactory();

    pico::ApplicationContainer* applicationContainer = factory->getApplicationContainer();

    applicationContainer->onMain(); //Run main one-time setup code.

    //https://github.com/raspberrypi/pico-examples/blob/eca13acf57916a0bd5961028314006983894fc84/multicore/multicore_fifo_irqs/multicore_fifo_irqs.c#L53
    // We MUST start the other core before we enabled FIFO interrupts.
    // This is because the launch uses the FIFO's, enabling interrupts before
    // they are used for the launch will result in unexpected behaviour.
    //TODO I think this is why we sometimes crash on boot. We might need to rejig
    //TODO how we start up the cores.

    multicore_reset_core1();
    multicore_launch_core1(core1_entry); //Launch the coprocessor and have it block.
//    Push the pointer to the application container to the second core. This unblocks the co-processor.
    multicore_fifo_push_blocking((uint32_t)applicationContainer);

    //Wait for the CPU1 setup to complete.
    sleep_ms(3000);


    while (true) {

        applicationContainer->onLoop();
//        sleep_ms(50);

//        const float voltage = adc_read() * conversion_factor;
//        const float temperature = 27 - (voltage - 0.706) / 0.001721;
//
//        printf("Hello, world! The temperature is: %fc\n", temperature);
    }
}
