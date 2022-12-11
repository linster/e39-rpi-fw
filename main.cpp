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

/*
    A simple application to blink the LED light, and print out the temperature.
*/
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

    sleep_ms(1000);
    applicationContainer->onMain();

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
