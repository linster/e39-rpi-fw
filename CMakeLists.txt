cmake_minimum_required(VERSION 3.16)

include(pico-sdk/pico_sdk_init.cmake)
include(pico-extras/external/pico_extras_import.cmake)


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




#https://github.com/google/fruit/issues/62#issuecomment-383369945
#https://github.com/google/fruit/issues/121

pico_sdk_init()

add_executable(PicoTemplate
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
        main.cpp
        ibus/observerRegistry/observers/telephoneLongPress/TelephoneLongPressObserver.cpp
        ibus/observerRegistry/observers/telephoneLongPress/TelephoneLongPressObserver.h
        ibus/outputWriterApi/ibusLoggerOutput/IbusLoggerOutput.cpp
        ibus/outputWriterApi/ibusLoggerOutput/IbusLoggerOutput.h
        ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.cpp
        ibus/outputWriterApi/screenPowerManager/ScreenPowerManager.h
        ibus/observerRegistry/observers/picoConfigurationReadRequest/picoConfigurationReadRequestObserver.cpp
        ibus/observerRegistry/observers/picoConfigurationReadRequest/picoConfigurationReadRequestObserver.h
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
        proto_generated/PiToPico.pb.c proto_cpp/ConfigMessage.cpp proto_cpp/ConfigMessage.h proto_cpp/PicoToPi.cpp proto_cpp/PicoToPi.h proto_cpp/PiToPico.cpp proto_cpp/PiToPico.h)

# Link with the pico stdlib
target_link_libraries(PicoTemplate
        fmt::fmt-header-only
        nanopb
        nanopb_cpp
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