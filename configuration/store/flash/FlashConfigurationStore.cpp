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
        return std::pair<bool, std::vector<uint8_t>>(false, std::vector<uint8_t>());
//            NanoPb::StringOutputStream outputStream;
//
//
//            if (!NanoPb::encode<messages::ConfigMessageConverter>(outputStream, configuration)) {
//                //TODO WTF
//                return;
//            }
//
//            std::unique_ptr<std::basic_string<char>> p = outputStream.release();
////                    p->c_str();
////                    p->length();
//
//            auto bytes = std::vector<uint8_t>();
//            for (char c: *p) {
//                bytes.push_back(c);
//            }
//
//            return bytes;
    }

    uint8_t *FlashConfigurationStore::getPointerToConfigurationStorageInFlash() {
        return nullptr;
    }

    uint16_t FlashConfigurationStore::getLengthOfConfigurationStorageInFlash() {
        return 0;
    }

    void FlashConfigurationStore::saveConfigurationBytes(uint8_t * ptr, std::vector<uint8_t>) {

    }

    std::pair<bool, Configuration> FlashConfigurationStore::decodeConfiguration(uint8_t *ptr, uint16_t len) {
        return std::pair<bool, Configuration>(false, Configuration());
    }


} // config