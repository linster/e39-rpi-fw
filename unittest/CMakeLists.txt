

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        unit_test
        ibus/dma/Packetizer_Test.cpp
)

target_sources(
        unit_test
        PUBLIC
        ibus/dma/Packetizer_Test.cpp
        ibus/outputWriterApi/IbusLoggerOutputTest.cpp
        observerRegistry/observers/BaseObserverPiToPicoMessageDecodeTests.cpp
)


target_link_libraries(
        unit_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_test)