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

        uint8_t* flashPtr = (uint8_t*)(XIP_BASE + PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE);

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
        //While the flash is being erased/programmed, XIP is off, so nothing on either
        //core may execute from (or read) flash. That means:
        // - interrupts off on this core (the UART RX ISRs live in flash), and
        // - core 1 parked in RAM via multicore lockout, if core 1 is running our code.

        uint32_t flash_offset = PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE;

        if (config.empty()
            || config.size() > FLASH_SECTOR_SIZE
            || (config.size() % FLASH_PAGE_SIZE) != 0) {
            logger->w("FlashConfigurationStore",
                      fmt::format("saveConfigurationBytes refusing bad size {}", config.size()));
            return;
        }

        //Core 1 calls multicore_lockout_victim_init() as soon as it starts (main.cpp core1_entry).
        //If it hasn't, core 1 is either never launched (no-video build) or still parked in the
        //bootrom (we're in onMain() before multicore_launch_core1), so it can't touch flash.
        bool lockOutCore1 = multicore_lockout_victim_is_initialized(1);

        if (lockOutCore1) {
            if (!multicore_lockout_start_timeout_us(500 * 1000)) {
                logger->e("FlashConfigurationStore",
                          "saveConfigurationBytes could not lock out core 1; not writing flash");
                return;
            }
        }

        uint32_t interrupts = save_and_disable_interrupts();
        flash_range_erase(flash_offset, FLASH_SECTOR_SIZE);
        flash_range_program(flash_offset, config.data(), config.size());
        restore_interrupts(interrupts);

        if (lockOutCore1) {
            multicore_lockout_end_blocking();
        }

        logger->d("FlashConfigurationStore",
                  fmt::format("saveConfigurationBytes wrote {} bytes", config.size()));
    }

} // config