//
// Created by stefan on 4/5/23.
//

#include "FlashConfigurationStore.h"

namespace pico::config {

    FlashConfigurationStore::FlashConfigurationStore(
            std::shared_ptr<logger::BaseLogger> logger) {
        this->logger = logger;
    }

    bool FlashConfigurationStore::canReadConfiguration() {
//        return false; //We're not fixing config right now
        std::pair<bool, Configuration> result = decodeConfiguration();
        return result.first;
    }

    Configuration* FlashConfigurationStore::getConfiguration() {
        logger->d("FlashConfigurationStore", "getConfiguration()");
        std::pair<bool, Configuration> result = decodeConfiguration();
        decodedConfiguration = result.second;
        return &decodedConfiguration;
    }

    void FlashConfigurationStore::saveConfiguration(Configuration configuration) {

//        return;
        std::pair<bool, std::vector<uint8_t>> encodeResult = encodeConfiguration(configuration);

        if (!encodeResult.first) {
            logger->w("FlashConfigurationStore", "Failed to encode message to save,");
            return;
        }

        saveConfigurationBytes(encodeResult.second);
    }

    std::pair<bool, std::vector<uint8_t>> FlashConfigurationStore::encodeConfiguration(
            Configuration configuration
    ) {
        bool encodeSuccess = false;

        auto outputStream = NanoPb::StringOutputStream(getLengthOfConfigurationStorageInFlash());

        encodeSuccess = NanoPb::encode<messages::ConfigMessageConverter>(
                outputStream, configuration.toMessage());

        if (!encodeSuccess) {
            logger->w("FlashConfigurationStore", "encodeConfiguration encodeSuccess was false");
            return {false, std::vector<uint8_t>()};
        }

        std::unique_ptr<std::basic_string<char>> p = outputStream.release();
        auto bytes = std::vector<uint8_t>();
        for (char c: *p) {
            bytes.push_back(c);
        }

        //TODO here we need to make sure that bytes.size() is a multiple of 256 so that the flash write works.

        return {true, bytes};
    }

    uint8_t* FlashConfigurationStore::getPointerToConfigurationStorageInFlash() {
        return getAddressPersistent();
    }

    uint16_t FlashConfigurationStore::getLengthOfConfigurationStorageInFlash() {
        return 4096;
    }


    std::pair<bool, Configuration> FlashConfigurationStore::decodeConfiguration() {
        //If the flash is initialized with junk, we won't be able to decode it,
        //so no need to worry about memsetting it to 0s

        uint8_t* flashPtr = (uint8_t*)(XIP_BASE + PICO_FLASH_SIZE_BYTES - (4 * 1024));

        bool decodeSuccess = false;

        //A buffer to that backs the stream that NanoPB uses.
        std::vector<uint8_t> flashBytes = std::vector<uint8_t>();
        for (int i = 0; i < 4096; i++) {
            flashBytes[i] = flashPtr[i];
        }

        //Another copy, sadly. RIP 4kb of memory.
        std::string flashString = std::string(flashBytes.begin(), flashBytes.end());
        auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(flashString));

        messages::ConfigMessage decoded;
        decodeSuccess = NanoPb::decode<messages::ConfigMessageConverter>(inputStream, decoded);
        if (!decodeSuccess) {
            return {false, Configuration()};
        }
        return {!decoded.rpiFwGitCommitHash.empty(), Configuration(decoded)};
    }

    void FlashConfigurationStore::saveConfigurationBytes(std::vector<uint8_t> config) {

        //https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#hardware_flash
        //https://www.raspberrypi.com/documentation/pico-sdk/high_level.html#multicore_lockout
        //I think I need to do the startup manager work first, so that I can set up Core1 as the victim core.
#if CMAKE_HAS_VIDEO_SUPPORT == CMAKE_VIDEO_SUPPORT_HAS_VIDEO
        multicore_lockout_start_blocking();
#endif

//        uint32_t flash_offset = (uint32_t)(&getAddressPersistent()[0]) - XIP_BASE;
//        uint32_t flash_offset = (uint32_t)(&getAddressPersistent()[0]);


//        __PERSISTENT_STORAGE_LEN = 4k ;
//        FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2048k - __PERSISTENT_STORAGE_LEN
//        FLASH_PERSISTENT(rw) : ORIGIN = 0x10000000 + (2048k - __PERSISTENT_STORAGE_LEN) , LENGTH = __PERSISTENT_STORAGE_LEN
//        PICO_FLASH_SIZE_BYTES;

//        uint32_t flash_offset = XIP_BASE + PICO_FLASH_SIZE_BYTES - (4 * 1024) ;
        uint32_t flash_offset = PICO_FLASH_SIZE_BYTES - (4 * 1024) ;


        flash_range_erase(flash_offset, 1 /* 4096 bytes, the size of our persistent block */);
        flash_range_program(flash_offset, config.data(), config.size()); //TODO this has to be a 256 byte aligned size?

#if CMAKE_HAS_VIDEO_SUPPORT == CMAKE_VIDEO_SUPPORT_HAS_VIDEO
        multicore_lockout_end_blocking();
#endif
    }

} // config