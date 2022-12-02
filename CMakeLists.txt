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
        VGADemo.cpp
        VGADemo.h
        logging/BaseLogger.cpp
        logging/BaseLogger.h
        logging/StdioPrintFLogger.cpp
        logging/StdioPrintFLogger.h
        video/scanProgram/scanPrograms/bmbtVGATiming.cpp
        video/scanProgram/scanPrograms/BaseScanProgram.cpp
        configuration/store/IConfigurationStore.cpp
        configuration/store/IConfigurationStore.h
        configuration/store/IConfigurationStore.cpp
        configuration/store/IConfigurationStore.h
        configuration/store/memory/MemoryConfigurationStore.cpp
        configuration/store/memory/MemoryConfigurationStore.h
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
        hardware/videoSwitch/max4314/Max4314VideoSwitch.cpp
        hardware/videoSwitch/max4314/Max4314VideoSwitch.h
        hardware/videoSwitch/mock/MockVideoSwitch.cpp
        hardware/videoSwitch/mock/MockVideoSwitch.h
        factory/ApplicationContainer.cpp
        factory/ApplicationContainer.h
        factory/pimoroniFactory/PimoroniFactory.cpp
        factory/pimoroniFactory/PimoroniFactory.h
        factory/e39RpiFactory/e39rpiFactory.cpp
        factory/e39RpiFactory/e39rpiFactory.h
        logging/CompositeLogger.cpp
        logging/CompositeLogger.h
        logging/ibusOutLogger/IBusOutLogger.cpp
        logging/ibusOutLogger/IBusOutLogger.h
        ibus/data/IBusDevices.h
        ibus/data/IbusPacket.cpp
        ibus/data/IbusPacket.h
        ibus/data/IbusDeviceEnum.cpp
        ibus/data/IbusDeviceEnum.h
        ibus/observerRegistry/observers/BaseObserver.cpp
        ibus/observerRegistry/observers/BaseObserver.h
        ibus/observerRegistry/ObserverRegistry.cpp
        ibus/observerRegistry/ObserverRegistry.h
        ibus/outputWriterApi/BaseOutputWriter.cpp
        ibus/outputWriterApi/BaseOutputWriter.h
        ibus/observerRegistry/observers/mockObserver/MockObserver.cpp
        ibus/observerRegistry/observers/mockObserver/MockObserver.h
        ibus/observerRegistry/observers/knobListener/KnobListenerObserver.cpp
        ibus/observerRegistry/observers/knobListener/KnobListenerObserver.h
        ibus/observerRegistry/observers/ignitionState/IgnitionStateObserver.cpp
        ibus/observerRegistry/observers/ignitionState/IgnitionStateObserver.h
        main.cpp)

# Link with the pico stdlib
target_link_libraries(PicoTemplate
        fmt::fmt-header-only
        pico_stdlib
        hardware_adc
        pico_multicore
        pico_stdlib
        pico_scanvideo_dpi)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(PicoTemplate)

# Enable USB serial
pico_enable_stdio_usb(PicoTemplate 1)
pico_enable_stdio_uart(PicoTemplate 0)