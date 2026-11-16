//
// On-flash framing for the persisted configuration.
//
// Layout of the 4k persistent block:
//   [0..3]  magic "E39C"
//   [4..5]  payload length, little-endian
//   [6..]   nanopb-encoded ConfigMessage (payload length bytes)
//   rest    zero padding up to a multiple of FLASH_PAGE_SIZE (256)
//
// The length is needed because nanopb's pb_decode() rejects trailing zero
// padding ("zero tag"), and erased flash (0xFF) is not a valid message either.
// No Pico SDK dependencies so this can be unit-tested on the host.
//

#ifndef PICOTEMPLATE_FLASHCONFIGURATIONFRAMING_H
#define PICOTEMPLATE_FLASHCONFIGURATIONFRAMING_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace pico::config::flash {

    static constexpr uint8_t FRAME_MAGIC[4] = {'E', '3', '9', 'C'};
    static constexpr size_t FRAME_HEADER_LEN = 6;
    static constexpr size_t FRAME_PAGE_SIZE = 256;     //FLASH_PAGE_SIZE
    static constexpr size_t FRAME_REGION_SIZE = 4096;  //FLASH_SECTOR_SIZE, the whole persistent block
    static constexpr size_t FRAME_MAX_PAYLOAD = FRAME_REGION_SIZE - FRAME_HEADER_LEN;

    /**
     * Wrap an encoded payload into a page-aligned buffer ready for flash_range_program().
     * @return empty if the payload doesn't fit in the persistent block.
     */
    inline std::optional<std::vector<uint8_t>> frameConfiguration(const std::string &payload) {
        if (payload.size() > FRAME_MAX_PAYLOAD) {
            return std::nullopt;
        }

        size_t used = FRAME_HEADER_LEN + payload.size();
        size_t padded = ((used + FRAME_PAGE_SIZE - 1) / FRAME_PAGE_SIZE) * FRAME_PAGE_SIZE;

        std::vector<uint8_t> out(padded, 0);
        for (size_t i = 0; i < sizeof(FRAME_MAGIC); i++) {
            out[i] = FRAME_MAGIC[i];
        }
        out[4] = static_cast<uint8_t>(payload.size() & 0xFF);
        out[5] = static_cast<uint8_t>((payload.size() >> 8) & 0xFF);
        for (size_t i = 0; i < payload.size(); i++) {
            out[FRAME_HEADER_LEN + i] = static_cast<uint8_t>(payload[i]);
        }
        return out;
    }

    /**
     * Extract the encoded payload from a raw view of the persistent block.
     * @return empty if the block has no valid frame (e.g. erased flash, old format).
     */
    inline std::optional<std::string> unframeConfiguration(const uint8_t *region, size_t regionLen) {
        if (region == nullptr || regionLen < FRAME_HEADER_LEN) {
            return std::nullopt;
        }
        for (size_t i = 0; i < sizeof(FRAME_MAGIC); i++) {
            if (region[i] != FRAME_MAGIC[i]) {
                return std::nullopt;
            }
        }
        size_t len = static_cast<size_t>(region[4]) | (static_cast<size_t>(region[5]) << 8);
        if (len > FRAME_MAX_PAYLOAD || FRAME_HEADER_LEN + len > regionLen) {
            return std::nullopt;
        }
        const char *start = reinterpret_cast<const char *>(region + FRAME_HEADER_LEN);
        return std::string(start, len);
    }

} // pico::config::flash

#endif //PICOTEMPLATE_FLASHCONFIGURATIONFRAMING_H
