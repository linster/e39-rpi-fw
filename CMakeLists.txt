cmake_minimum_required(VERSION 3.16)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-extras/external/pico_extras_import.cmake)


add_subdirectory(libs/fmt)

project(PicoTemplate)


#https://github.com/google/fruit/issues/62#issuecomment-383369945
#https://github.com/google/fruit/issues/121

pico_sdk_init()

add_executable(PicoTemplate
        libs/fmt
        main.cpp
        VGADemo.cpp
        VGADemo.h
        logging/BaseLogger.cpp
        logging/BaseLogger.h
        logging/StdioPrintFLogger.cpp
        logging/StdioPrintFLogger.h
        video/scanProgram/scanPrograms/bmbtVGATiming.cpp
        video/scanProgram/scanPrograms/BaseScanProgram.cpp
        configuration/IConfigurationStore.cpp
        configuration/IConfigurationStore.h
        configuration/IConfigurationStore.cpp
        configuration/IConfigurationStore.h
        configuration/MemoryConfigurationStore.cpp
        configuration/MemoryConfigurationStore.h
        configuration/Configuration.cpp
        configuration/Configuration.h
        configuration/ConfigurationManager.cpp
        configuration/ConfigurationManager.h
        hardware/pi4powerswitch/GpioPi4PowerSwitchManager.cpp
        hardware/pi4powerswitch/GpioPi4PowerSwitchManager.h
        hardware/pi4powerswitch/MockPi4PowerSwitchManager.cpp
        hardware/pi4powerswitch/MockPi4PowerSwitchManager.h
        hardware/pi4powerswitch/IPi4PowerSwitchManager.cpp
        hardware/pi4powerswitch/IPi4PowerSwitchManager.h
        hardware/videoSwitch/VideoSource.h
        hardware/videoSwitch/VideoSource.h
        hardware/videoSwitch/VideoSwitch.cpp
        hardware/videoSwitch/max4314/Max4314VideoSwitch.cpp hardware/videoSwitch/max4314/Max4314VideoSwitch.h hardware/videoSwitch/mock/MockVideoSwitch.cpp hardware/videoSwitch/mock/MockVideoSwitch.h)

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