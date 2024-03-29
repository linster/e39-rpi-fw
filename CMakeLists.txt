cmake_minimum_required(VERSION 3.16)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-extras/external/pico_extras_import.cmake)

set(CMAKE_CXX_STANDARD 17)

project(PicoTemplate)


add_subdirectory(libs/fmt)
add_subdirectory(libs/nanopb)
add_subdirectory(libs/nanopb_cpp)


#stefan@pop-os:~/pico/protobuf$
#./protoc --plugin=protoc-gen-nanopb=/home/stefan/e39-rpi-fw/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=/home/stefan/e39-rpi-fw/proto_generated /home/stefan/e39-rpi-fw/configuration/ConfigProto.proto_generated --proto_path=/home/stefan/e39-rpi-fw

#This one works.
# TODO.
#./protoc --plugin=protoc-gen-nanopb=/home/stefan/e39-rpi-fw/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=/home/stefan/e39-rpi-fw/proto_generated /home/stefan/e39-rpi-fw/configuration/ConfigProto.proto_generated --proto_path=/home/stefan/e39-rpi-fw/configuration/

#TODO this one worked. It had a goofy error, but w/e
#./protoc --plugin=protoc-gen-nanopb=/home/stefan/e39-rpi-fw/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=/home/stefan/e39-rpi-fw/proto_generated /home/stefan/e39-rpi-fw/proto/PiToPico.proto --proto_path=/home/stefan/e39-rpi-fw/proto/
#/home/stefan/e39-rpi-fw/libs/nanopb/generator/proto/nanopb.proto_generated: No such file or directory



#https://jaredrhodes.com/2018/08/24/generate-protocol-buffers-on-build-with-cmake/
#file(GLOB_RECURSE PROTOBUF_DEFINITION_FILES "*.proto_generated")
#set(PROTOBUF_INPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")
#set(PROTOBUF_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/Proto/")
#foreach(file ${PROTOBUF_DEFINITION_FILES})
#
#    set(outputPath ${file})
#    ##close. This has to be one level up
#    cmake_path(GET outputPath PARENT_PATH outputPath)
#    #I wonder if we do configuration/foo/bar if we have to strip out like crazy.
#    #Maybe it's just better to use the project path?
#    cmake_path(GET outputPath PARENT_PATH outputPath)
#    #    cmake_path(NORMALIZE outputPath)
#    set(PROTOBUF_ARGUMENTS "protoc --proto_path=\"${PROTOBUF_INPUT_DIRECTORY}\" --cpp_out=\"${outputPath}\" \"${file}\"")
#    #    set(PROTOBUF_ARGUMENTS "protoc --proto_path=\"${PROTOBUF_INPUT_DIRECTORY}\" --cpp_out=\"${PROTOBUF_OUTPUT_DIRECTORY}\" \"${file}\"")
#
#
#    execute_process(COMMAND ${PROTOBUF_OUTPUT_DIRECTORY}
#            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#            RESULT_VARIABLE PROTOBUF_RESULT
#            OUTPUT_VARIABLE PROTOBUF_OUTPUT_VARIABLE)
#endforeach()
#
#file(GLOB_RECURSE PROTOBUF_MODELS_INCLUDES "Proto/*.pb.cc" "Proto/*.hpp")
#end proto_generated


#stefan@pop-os:~/e39-rpi-fw$ ./libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=proto_generated proto/PiToPico.proto --proto_path=proto/


#https://github.com/google/fruit/issues/62#issuecomment-383369945
#https://github.com/google/fruit/issues/121

pico_sdk_init()

#TODO maybe I need to splitup add_executable?? add_executable, then generate pio header, then target_sources?

add_executable(PicoTemplate)

message("Shell command for generating PiToPico.pb.h")
message("PiToPico.pb.h: ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/PiToPico.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/")

message("Shell command for generating PicoToPi.pb.h")
message("PicoToPi.pb.h: ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/PicoToPi.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/")

message("Shell command for generating ConfigProto.pb.h")
message("ConfigProto.pb.h: ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/ConfigProto.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/")

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/PiToPico.pb.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/PiToPico.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/
        COMMENT "Generating PiToPico.pb.h"
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/PicoToPi.pb.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/PicoToPi.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/
        COMMENT "Generating PicoToPi.pb.h"
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/ConfigProto.pb.h
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc --plugin=protoc-gen-nanopb=${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb/generator/protoc-gen-nanopb --nanopb_out=${CMAKE_CURRENT_SOURCE_DIR}/proto_generated ${CMAKE_CURRENT_SOURCE_DIR}/proto/ConfigProto.proto --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/proto/
        COMMENT "Generating ConfigProto.pb.h"
)

add_custom_target(
        GenerateProtobufHeaders ALL
        DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/PiToPico.pb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/PicoToPi.pb.h
        ${CMAKE_CURRENT_SOURCE_DIR}/proto_generated/ConfigProto.pb.h
)

add_dependencies(PicoTemplate GenerateProtobufHeaders)

target_sources(
        PicoTemplate PUBLIC
        libs/fmt
        libs/nanopb/pb.h
        libs/nanopb/pb_common.h
        libs/nanopb/pb_common.c
        libs/nanopb/pb_decode.h
        libs/nanopb/pb_decode.c
        libs/nanopb/pb_encode.h
        libs/nanopb/pb_encode.c
        libs/nanopb_cpp/nanopb_cpp.h
        libs/nanopb_cpp/nanopb_cpp.cpp
        logging/BaseLogger.cpp
        logging/BaseLogger.h
        logging/StdioPrintFLogger.cpp
        logging/StdioPrintFLogger.h

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

        logging/CompositeLogger.cpp
        logging/CompositeLogger.h
        logging/ibusOutLogger/IBusOutLogger.cpp
        logging/ibusOutLogger/IBusOutLogger.h
        logging/factory/LoggerFactory.cpp
        logging/factory/LoggerFactory.h
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

        ibus/observerRegistry/observers/PiToPico/ConfigPush/PicoConfigPushObserver.cpp
        ibus/observerRegistry/observers/PiToPico/ConfigPush/PicoConfigPushObserver.h


        main.cpp

        ibus/observerRegistry/observers/uartForwarderObserver/UartForwarderObserver.cpp
        ibus/observerRegistry/observers/uartForwarderObserver/UartForwarderObserver.h
        ibus/outputWriterApi/uartForwarderWriter/UartForwarderWriter.cpp
        ibus/outputWriterApi/uartForwarderWriter/UartForwarderWriter.h
        ibus/outputWriterApi/PicoToPi/ibusLoggerOutput/IbusLoggerOutput.cpp
        ibus/outputWriterApi/PicoToPi/ibusLoggerOutput/IbusLoggerOutput.h
        ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.cpp
        ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h
        ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.cpp
        ibus/observerRegistry/observers/PiToPico/picoConfigurationReadRequest/PicoConfigurationReadRequestObserver.h
        configuration/default/DefaultConfigurationProvider.cpp
        configuration/default/DefaultConfigurationProvider.h
        configuration/default/PiDefaultConfigurationProvider.cpp
        configuration/default/PiDefaultConfigurationProvider.h
        configuration/default/PimoroniDefaultConfigurationProvider.cpp
        configuration/default/PimoroniDefaultConfigurationProvider.h
        proto_generated/ConfigProto.pb.h
        proto_generated/ConfigProto.pb.c
        proto_generated/PicoToPi.pb.h
        proto_generated/PicoToPi.pb.c
        proto_generated/PiToPico.pb.h
        proto_generated/PiToPico.pb.c
        proto_cpp/ConfigMessage.cpp
        proto_cpp/ConfigMessage.h
        proto_cpp/PicoToPi.cpp
        proto_cpp/PicoToPi.h
        proto_cpp/PiToPico.cpp
        proto_cpp/PiToPico.h
        configuration/store/flash/FlashConfigurationStore.cpp
        configuration/store/flash/FlashConfigurationStore.h
        configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.cpp
        configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.h
        configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.cpp
        configuration/store/ibusConfigMessageStore/IBusConfigMessageStore.h
        configuration/store/flash/FlashConfigurationStore.h
        configuration/store/flash/FlashConfigurationStore.cpp
        configuration/store/flash/FlashConfigurationStore.cpp
        configuration/store/flash/flash_utils.h
        ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.cpp
        ibus/outputWriterApi/PicoToPi/picoConfigurationStatusWriter/ConfigurationStatusWriter.h
        ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.cpp
        ibus/outputWriterApi/PicoToPi/picoPiSoftPowerRequest/SoftPowerRequestWriter.h
        ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.cpp
        ibus/observerRegistry/observers/PiToPico/heartbeatRequest/HeatbeatRequestObserver.h
        ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.cpp
        ibus/observerRegistry/observers/PiToPico/PiPowerRequest/PiPowerRequestObserver.h
        ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.cpp
        ibus/observerRegistry/observers/PiToPico/VideoRequest/PicoVideoRequestObserver.h
        ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.cpp
        ibus/outputWriterApi/PicoToPi/heartbeatResponseWriter/HeartbeatResponseWriter.h
        ibus/dma/PioPins.h
        ibus/dma/Packetizer.cpp
        ibus/dma/Packetizer.h

        ibus/dma/IDmaManager.h
        ibus/dma/IDmaManager.cpp
        ibus/dma/SingleCoreDmaManager.cpp
        ibus/dma/SingleCoreDmaManager.h
        ibus/outputWriterApi/TestingOutputWriter.h
        ibus/outputWriterApi/TestingOutputWriter.cpp
        ibus/outputWriterApi/radioControl/AudioFocusOutputWriter.h
        ibus/outputWriterApi/radioControl/AudioFocusOutputWriter.cpp

        watchdog/WatchdogManager.h
        watchdog/WatchdogManager.cpp

        ibus/dma/topology/PicoBusTopology.h
        ibus/dma/topology/BusTopologyManager.h
        ibus/dma/topology/BusTopologyManager.cpp
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/ibus/dma/topology/CMakeHeaderFlags.h>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/video/CMakeHasVideoFlags.h>
)

target_include_directories(
        PicoTemplate
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/nanopb>
#        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ibus>
)

pico_set_linker_script(PicoTemplate ${CMAKE_SOURCE_DIR}/configuration/store/flash/memmap_persistent.ld)

target_link_libraries(PicoTemplate hardware_flash hardware_sync)

#https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf -- page 36
#pico_generate_pio_header(PicoTemplate ${CMAKE_SOURCE_DIR}/ibus/dma/pio/FromCarProgram.pio)
#pico_generate_pio_header(PicoTemplate ${CMAKE_CURRENT_LIST_DIR}/ibus/dma/pio/ToCarProgram.pio)

# Link with the pico stdlib
target_link_libraries(PicoTemplate
        fmt::fmt-header-only
#        nanopb
#        nanopb_cpp
        pico_stdlib
        hardware_adc
        hardware_pio
        pico_multicore
        pico_stdlib
        )

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(PicoTemplate)

# Support for video on Pico
# Disabled to allow the software to be stable enough to run in a real car (because the clock speed cause DMA starvation,
# which causes isr_hardfaults)
set(CMAKE_VIDEO_SUPPORT_NO_VIDEO 0)
set(CMAKE_VIDEO_SUPPORT_HAS_VIDEO 1)

set(CMAKE_HAS_VIDEO_VALUE CMAKE_VIDEO_SUPPORT_NO_VIDEO)
#set(CMAKE_BUS_HAS_VIDEO_VALUE CMAKE_VIDEO_SUPPORT_HAS_VIDEO)

#This is really dumb, but it needs to be here for #cmakedefine to actually run.
set(CMAKE_HAS_VIDEO_SUPPORT 1)

configure_file(
        video/CMakeHasVideoFlags.in.h
        video/CMakeHasVideoFlags.h
)
target_include_directories(
        PicoTemplate
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/video>
)

if(${CMAKE_HAS_VIDEO_VALUE} EQUAL CMAKE_VIDEO_SUPPORT_NO_VIDEO)
    message("Setting up video support for CMAKE_VIDEO_SUPPORT_NO_VIDEO")


    target_sources(
            PicoTemplate PUBLIC

            factory/ApplicationContainerNoVideo.cpp
            factory/ApplicationContainerNoVideo.h
            factory/noVideoFactory/NoVideoFactory.cpp
            factory/noVideoFactory/NoVideoFactory.h

            ibus/observerRegistry/observers/ignitionStateNoVideo/IgnitionStateNoVideoObserver.cpp
            ibus/observerRegistry/observers/ignitionStateNoVideo/IgnitionStateNoVideoObserver.h

            ibus/observerRegistry/observers/telephonePressNoVideo/TelephonePressNoVideo.cpp
            ibus/observerRegistry/observers/telephonePressNoVideo/TelephonePressNoVideo.h
    )

endif()

if(${CMAKE_HAS_VIDEO_VALUE} EQUAL CMAKE_VIDEO_SUPPORT_HAS_VIDEO)
    message("Setting up video support for CMAKE_VIDEO_SUPPORT_HAS_VIDEO")


    target_link_libraries(PicoTemplate pico_scanvideo_dpi)


    target_sources(
            PicoTemplate PUBLIC

            factory/ApplicationContainer.cpp
            factory/ApplicationContainer.h
            factory/pimoroniFactory/PimoroniFactory.cpp
            factory/pimoroniFactory/PimoroniFactory.h
            factory/e39RpiFactory/e39rpiFactory.cpp
            factory/e39RpiFactory/e39rpiFactory.h

            ibus/observerRegistry/observers/ignitionState/IgnitionStateObserver.cpp
            ibus/observerRegistry/observers/ignitionState/IgnitionStateObserver.h

            ibus/observerRegistry/observers/telephoneLongPress/TelephoneLongPressObserver.cpp
            ibus/observerRegistry/observers/telephoneLongPress/TelephoneLongPressObserver.h

            ibus/observerRegistry/observers/knobListener/KnobListenerObserver.cpp
            ibus/observerRegistry/observers/knobListener/KnobListenerObserver.h

            video/screenManager/ScreenItem.cpp
            video/screenManager/ScreenItem.h
            video/screenManager/Screen.cpp
            video/screenManager/Screen.h
            video/screenManager/ScreenManager.cpp
            video/screenManager/ScreenManager.h
            video/screenManager/mainScreen/MainScreen.cpp
            video/screenManager/mainScreen/MainScreen.h
            video/screenManager/mainScreen/TurnOnPi4MenuItem.cpp
            video/screenManager/mainScreen/TurnOnPi4MenuItem.h
            video/screenManager/mainScreen/SafePi4PowerOffMenuItem.cpp
            video/screenManager/mainScreen/SafePi4PowerOffMenuItem.h
            video/screenManager/mainScreen/HardPi4PowerOffMenuItem.cpp
            video/screenManager/mainScreen/HardPi4PowerOffMenuItem.h
            video/screenManager/mainScreen/RestartXMenuItem.cpp
            video/screenManager/mainScreen/RestartXMenuItem.h
            video/screenManager/mainScreen/VideoToRvcMenuItem.cpp
            video/screenManager/mainScreen/VideoToRvcMenuItem.h
            video/screenManager/mainScreen/VideoToUpstreamMenuItem.cpp
            video/screenManager/mainScreen/VideoToUpstreamMenuItem.h
            video/screenManager/mainScreen/VideoToPiMenuItem.cpp
            video/screenManager/mainScreen/VideoToPiMenuItem.h
            video/screenManager/mainScreen/ConfigPushMenuItem.cpp
            video/screenManager/mainScreen/ConfigPushMenuItem.h
            video/scanProgram/ScanProgram.h
            video/scanProgram/scanPrograms/BaseScanProgram.cpp
            video/scanProgram/scanPrograms/demo/DemoScanProgram.cpp
            video/scanProgram/scanPrograms/demo/DemoScanProgram.h
            video/scanProgram/scanPrograms/demo/DemoScanProgram.h
            video/scanProgram/scanPrograms/demo/DemoScanProgram.h
            video/scanProgram/scanPrograms/clock/ClockScanProgram.cpp
            video/scanProgram/scanPrograms/clock/ClockScanProgram.h
            video/scanProgram/scanPrograms/bootsplash/BootsplashScanProgram.cpp
            video/scanProgram/scanPrograms/bootsplash/BootsplashScanProgram.h
            video/scanProgram/scanPrograms/menu/MenuScanProgram.cpp
            video/scanProgram/scanPrograms/menu/MenuScanProgram.h
            video/scanProgram/scanPrograms/noop/NoopScanProgram.cpp
            video/scanProgram/scanPrograms/noop/NoopScanProgram.h
            video/scanProgram/ScanProgramSwapper.cpp
            video/scanProgram/ScanProgramSwapper.cpp
            video/scanProgram/ScanProgramManager.cpp
            video/scanProgram/scanPrograms/graphicsLib/graphicsLib.cpp
            video/scanProgram/scanPrograms/graphicsLib/graphicsLib.h
            video/scanProgram/scanPrograms/graphicsLib/DisplayMetrics.cpp
            video/scanProgram/scanPrograms/graphicsLib/DisplayMetrics.h
            video/scanProgram/scanPrograms/graphicsLib/command/BaseCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/BaseCommand.h
            video/scanProgram/scanPrograms/graphicsLib/command/LineCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/LineCommand.h
            video/scanProgram/scanPrograms/graphicsLib/command/FilledRectangleCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/FilledRectangleCommand.h
            video/scanProgram/scanPrograms/graphicsLib/command/EmptyRectangleCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/EmptyRectangleCommand.h
            video/scanProgram/scanPrograms/graphicsLib/command/TextCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/TextCommand.h
            video/scanProgram/scanPrograms/graphicsLib/command/PxCoord.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/PxCoord.h
            video/scanProgram/scanPrograms/graphicsLib/command/CommandProcessor.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/CommandProcessor.h
            video/scanProgram/scanPrograms/graphicsLib/command/RleRun.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/RleRun.h
            video/scanProgram/scanPrograms/graphicsLib/command/RleRunContainerCommand.cpp
            video/scanProgram/scanPrograms/graphicsLib/command/RleRunContainerCommand.h
            video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.h
            video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.cpp
            video/scanProgram/scanPrograms/graphicsLib/fonts/FontProvider.h
    )

endif()


#https://discourse.cmake.org/t/how-to-get-access-cmake-variable-in-a-header-file/7125
#https://cmake.org/cmake/help/latest/command/configure_file.html
# @value@ turns into a name substitution, TCL style for a variable
# in scope in a CMakeScript. Wild. Literally expecting set(value 5).

### See PicoBusTopology.h for details. Valid values are:
# CMAKE_BUS_TOPOLOGY_CAR_WITH_PI
# CMAKE_BUS_TOPOLOGY_SLED_NO_PI
# CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI

set(CMAKE_BUS_TOPOLOGY_CAR_WITH_PI 0)
set(CMAKE_BUS_TOPOLOGY_SLED_NO_PI 1)
set(CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI 2)

#set(CMAKE_BUS_TOPOLOGY_VALUE CMAKE_BUS_TOPOLOGY_CAR_WITH_PI)
#set(CMAKE_BUS_TOPOLOGY_VALUE CMAKE_BUS_TOPOLOGY_SLED_NO_PI)
set(CMAKE_BUS_TOPOLOGY_VALUE CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI)

#This is really dumb, but it needs to be here for #cmakedefine to actually run.
set(CMAKE_BUS_TOPOLOGY 1)

message("CMAKE_BUS_TOPOLOGY_VALUE ${CMAKE_BUS_TOPOLOGY_VALUE}")

configure_file(
        ibus/dma/topology/CMakeHeaderFlags.in.h
        ibus/dma/topology/CMakeHeaderFlags.h
)
target_include_directories(
        PicoTemplate
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/ibus/dma/topology>
)

#Why the heck do you need to expand LHS but not the RHS of an equality statement??
if(${CMAKE_BUS_TOPOLOGY_VALUE} EQUAL CMAKE_BUS_TOPOLOGY_SLED_NO_PI)
    #Print everything over uart 1 for debugging.
    message("Setting up stdio for CMAKE_BUS_TOPOLOGY_SLED_NO_PI")
    pico_enable_stdio_usb(PicoTemplate 0)
    pico_enable_stdio_uart(PicoTemplate 1)
endif()

if(${CMAKE_BUS_TOPOLOGY_VALUE} EQUAL CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI)
    #Raw IBUS on UART, no stdio logs on uart. Stdio goes on usb.
    message("Setting up stdio for CMAKE_BUS_TOPOLOGY_SLED_LAPTOP_HMI")
    pico_enable_stdio_usb(PicoTemplate 0)
    pico_enable_stdio_uart(PicoTemplate 0)
endif()

if(${CMAKE_BUS_TOPOLOGY_VALUE} EQUAL CMAKE_BUS_TOPOLOGY_CAR_WITH_PI)
    #Raw IBUS on UART, no stdio logs on uart. Stdio goes on usb.
    message("Setting up stdio for CMAKE_BUS_TOPOLOGY_CAR_WITH_PI")
    pico_enable_stdio_usb(PicoTemplate 1)
    pico_enable_stdio_uart(PicoTemplate 0)
endif()
#Set PicoVGA Params

#target_compile_definitions(PicoTemplate PUBLIC IS_TWO_BPP=false)

set(E39_RPI_VGA_SMALL_COLOR_DEPTH ON)
if (E39_RPI_VGA_SMALL_COLOR_DEPTH)

#target_compile_definitions(PicoTemplate PUBLIC IS_TWO_BPP=true)

#Try to set the vga pins to 2 bits per channel, and relocate the HSync, VSync pins.
#scanvideo.h sets these.
#not sure about wither I want to set any of the _dpi_ values?
#Assuming DPI == display parallel interface
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_RCOUNT=2)
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_GCOUNT=2)
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_BCOUNT=2)

#When RCount = 5, Rshift = 0
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_RSHIFT=0)
#When GCount = 5, GShift = 6 -- Rcount + 1
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_GSHIFT=2)
#When BCount = 5, Bshift = 11 -- RCount + GCount + 1. So, set to
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_PIXEL_BSHIFT=4)

#wtf, scanvideo_base.h sets a bunch of stuff too?

#when we had RGB555, this value was set to 16. So, let's set it to (2 + 2 + 2) -1 == 5
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_COLOR_PIN_BASE=0)
target_compile_definitions(PicoTemplate PUBLIC PICO_SCANVIDEO_COLOR_PIN_COUNT=6)
#PICO_SCANVIDEO_COLOR_PIN_BASE is still 0, so our base pin is still GP0
#Before the above change, we had PICO_SCANVIDEO_COLOR_PIN_COUNT == 16, which meant
#that PICO_SCANVIDEO_SYNC_PIN_BASE == 0 + 16 = 16. According to the schematic, this
#meant that HSYNC/VSYNC started on GP16. Now, they will start on GP7??
#I don't fully understand all the off-by-one errors that are built into these macros.
#I'm just matching up what I see.

endif()