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
        std::pair<bool, Configuration> result = decodeConfiguration(
                getPointerToConfigurationStorageInFlash(),
                getLengthOfConfigurationStorageInFlash()
        );

        return result.first;
    }

    Configuration FlashConfigurationStore::getConfiguration() {
        std::pair<bool, Configuration> result = decodeConfiguration(
                getPointerToConfigurationStorageInFlash(),
                getLengthOfConfigurationStorageInFlash()
        );

        return result.second;
    }

    void FlashConfigurationStore::saveConfiguration(Configuration configuration) {

        std::pair<bool, std::vector<uint8_t>> encodeResult = encodeConfiguration(configuration);

        if (!encodeResult.first) {
            logger->w("FlashConfigurationStore", "Failed to encode message to save,");
            return;
        }

        saveConfigurationBytes(getPointerToConfigurationStorageInFlash(), encodeResult.second);
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

        return {true, bytes};
    }

    uint32_t* FlashConfigurationStore::getPointerToConfigurationStorageInFlash() {
        return getAddressPersistent();
    }

    uint16_t FlashConfigurationStore::getLengthOfConfigurationStorageInFlash() {
        return 4096;
    }


    std::pair<bool, Configuration> FlashConfigurationStore::decodeConfiguration(char *ptr, uint16_t len) {
        //If the flash is initialized with junk, we won't be able to decode it,
        //so no need to worry about memsetting it to 0s

        bool decodeSuccess = false;

        //A buffer to that backs the stream that NanoPB uses.
        std::string inputString = std::string(len, '\0');

        std::string

        NanoPb::StringOutputStream();

        std::make_unique<std::string>(ptr, getLengthOfConfigurationStorageInFlash());

        for (const uint8_t item: data) {
            inputString.push_back(item);
        }
        auto inputStream = NanoPb::StringInputStream(std::make_unique<std::string>(inputString.c_str()));

        messages::ConfigMessage decoded;
        decodeSuccess = NanoPb::decode<messages::ConfigMessageConverter>(inputStream, decoded);
        if (!decodeSuccess) {
            return {false, Configuration()};
        }

        return {true, Configuration(decoded)};
    }

    void FlashConfigurationStore::saveConfigurationBytes(uint32_t *ptr, std::vector<uint8_t> config) {

    }


} // config