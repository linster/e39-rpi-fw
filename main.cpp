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


const float conversion_factor = 3.3f / (1 << 12);


void core1_entry() {
    //The very first thing we get on startup wait and block for a pointer to the application container.
    pico::ApplicationContainer* applicationContainer = (pico::ApplicationContainer*) multicore_fifo_pop_blocking();

    //One-time setup on the second core.
    applicationContainer->onCpu1Main();

    sleep_ms(4000); //Don't start looping until the main cpu0 loop starts.

    while(true) {
        applicationContainer->onCpu1Loop();
    }
}



int main() {

    printf("main() start");
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    adc_init();
    adc_gpio_init(26);
    adc_set_temp_sensor_enabled(true);

    adc_select_input(TEMP_ADC);

    auto* logger = new pico::logger::StdioPrintFLogger();

    logger->d("Wat", "Foo");


    auto* factory = new pico::di::PimoroniFactory();

    pico::ApplicationContainer* applicationContainer = factory->getApplicationContainer();

    multicore_launch_core1(core1_entry); //Launch the coprocessor and have it block.

    applicationContainer->onMain(); //Run main one-time setup code.
    //Wait for the CPU0 setup to complete.
    sleep_ms(3000);

    //Push the pointer to the application container to the second core. This unblocks the co-processor.
    multicore_fifo_push_blocking((uint32_t)applicationContainer);

    //Wait for the CPU1 setup to complete.
    sleep_ms(3000);

    while (true) {
        gpio_put(LED_PIN, true);
        applicationContainer->onLoop();
        gpio_put(LED_PIN, false);

//        const float voltage = adc_read() * conversion_factor;
//        const float temperature = 27 - (voltage - 0.706) / 0.001721;
//
//        printf("Hello, world! The temperature is: %fc\n", temperature);
    }
}
