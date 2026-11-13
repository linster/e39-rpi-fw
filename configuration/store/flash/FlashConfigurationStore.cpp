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

        //Prefix with magic + length (so decode knows where the message ends) and
        //pad to a multiple of 256 bytes so flash_range_program gets whole pages.
        std::optional<std::vector<uint8_t>> framed = flash::frameConfiguration(*p);
        if (!framed.has_value()) {
            logger->w("FlashConfigurationStore",
                      fmt::format("encodeConfiguration message too large: {} bytes", p->size()));
            return {false, std::vector<uint8_t>()};
        }

        return {true, framed.value()};
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

        //Only copy out the framed payload; erased flash or an old-format block has no valid frame.
        std::optional<std::string> payload = flash::unframeConfiguration(flashPtr, flash::FRAME_REGION_SIZE);
        if (!payload.has_value()) {
            return {false, Configuration()};
        }

        auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(std::move(payload.value())));

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