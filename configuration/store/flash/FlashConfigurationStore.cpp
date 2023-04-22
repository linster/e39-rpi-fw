//
// Created by stefan on 4/5/23.
//

#include "FlashConfigurationStore.h"

namespace pico {
    namespace config {

        bool FlashConfigurationStore::canReadConfiguration() {
            //TODO Try-catch reading a block of flash memory
            //No catch on this platform.
            return false;
        }

        Configuration FlashConfigurationStore::getConfiguration() {
            //TODO read the block of memory
        }

        void FlashConfigurationStore::saveConfiguration(Configuration configuration) {
            //TODO save the block


        }

        std::vector<uint8_t> FlashConfigurationStore::encodeConfiguration(
                Configuration configuration
        ) {
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

    } // pico
} // config