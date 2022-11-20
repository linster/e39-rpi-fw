cmake_minimum_required(VERSION 3.16)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-extras/external/pico_extras_import.cmake)

project(PicoTemplate)

pico_sdk_init()

add_executable(PicoTemplate main.cpp VGADemo.cpp VGADemo.h bmbtVGATiming.h logging/BaseLogger.cpp logging/BaseLogger.h logging/StdioPrintFLogger.cpp logging/StdioPrintFLogger.h)

# Link with the pico stdlib
target_link_libraries(PicoTemplate pico_stdlib hardware_adc
        pico_multicore
        pico_stdlib
        pico_scanvideo_dpi)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(PicoTemplate)

# Enable USB serial
pico_enable_stdio_usb(PicoTemplate 1)
pico_enable_stdio_uart(PicoTemplate 0)