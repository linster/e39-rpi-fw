//
// Bounded encoder for pico_scanvideo "composable" scanlines.
//
// Adapted from the pico-playground test_pattern.c pattern
// (runs, then RAW_1P black, then end-of-line), with two changes needed because we emit a
// variable number of 2- and 3-halfword runs instead of a fixed 32 x 3:
//
//  1. Bounds: never write more than the scanline buffer holds. If the runs don't fit, the
//     rest of the line is filled with one run of fillColour and the line is ended properly.
//  2. Alignment: per scanvideo.pio, end_of_scanline_skip_ALIGN must start on a word boundary
//     ("|| jmp | ignored ||") and end_of_scanline_ALIGN on an odd halfword ("| jmp ||").
//     test_pattern.c is always word-aligned there; we pick the token from the actual parity,
//     like scanvideo.c's own missing-scanline data does.
//
// No Pico SDK dependencies (token values are passed in) so it can be unit-tested on the host.
//

#ifndef PICOTEMPLATE_SCANLINEENCODER_H
#define PICOTEMPLATE_SCANLINEENCODER_H

#include <cstddef>
#include <cstdint>
#include <vector>
#include "RleRun.h"

namespace video::scanVideo::graphics::command {

    struct ScanlineTokens {
        uint16_t colorRun;      //COMPOSABLE_COLOR_RUN     | jmp | color | count-3 |
        uint16_t raw1p;         //COMPOSABLE_RAW_1P        | jmp | color |
        uint16_t raw2p;         //COMPOSABLE_RAW_2P        | jmp | color | color |
        uint16_t eolAlign;      //COMPOSABLE_EOL_ALIGN     | jmp ||           (starts on odd halfword)
        uint16_t eolSkipAlign;  //COMPOSABLE_EOL_SKIP_ALIGN || jmp | ignored || (starts on word boundary)
    };

    struct ScanlineEncodeResult {
        size_t halfwordsWritten = 0; //Always even when ok: the line ends on a word boundary.
        size_t wordsUsed = 0;        //Value for scanline_buffer->data_used.
        bool truncated = false;      //Runs didn't fit; rest of line was filled with fillColour.
        bool ok = false;             //False only if the buffer can't even hold an empty line.
    };

    //Halfwords always kept free for the end of the line: RAW_1P, 0, then EOL_SKIP_ALIGN, 0
    //(or EOL_ALIGN), plus one COLOR_RUN (3) to fill the rest of the line on truncation.
    static constexpr size_t SCANLINE_TRAILER_HALFWORDS = 4;
    static constexpr size_t SCANLINE_FILL_HALFWORDS = 3;

    inline size_t scanlineRunCost(uint16_t len) {
        if (len == 0) return 0;
        if (len == 1) return 2;
        return 3; //RAW_2P and COLOR_RUN are both 3 halfwords
    }

    inline void scanlineWriteRun(uint16_t *out, size_t &pos, const ScanlineTokens &t,
                                 uint16_t len, uint16_t colour) {
        if (len == 1) {
            out[pos++] = t.raw1p;
            out[pos++] = colour;
        } else if (len == 2) {
            out[pos++] = t.raw2p;
            out[pos++] = colour;
            out[pos++] = colour;
        } else if (len >= 3) {
            out[pos++] = t.colorRun;
            out[pos++] = colour;
            out[pos++] = len - 3;
        }
    }

    /**
     * Encode runs into a scanline buffer without exceeding capacityHalfwords.
     * @param runs          sorted, non-overlapping runs (as produced by CommandProcessor::mergeRuns)
     * @param out           scanline_buffer->data viewed as uint16_t
     * @param capacityHalfwords  scanline_buffer->data_max * 2
     * @param lineWidth     display width in pixels (for the truncation fill)
     * @param fillColour    colour for the rest of the line if the runs don't fit
     */
    inline ScanlineEncodeResult encodeScanline(
            const std::vector<RleRun> &runs,
            uint16_t *out,
            size_t capacityHalfwords,
            const ScanlineTokens &t,
            uint16_t lineWidth,
            uint16_t fillColour) {

        ScanlineEncodeResult result;
        if (out == nullptr || capacityHalfwords < SCANLINE_TRAILER_HALFWORDS + SCANLINE_FILL_HALFWORDS) {
            return result;
        }

        const size_t runBudget = capacityHalfwords - SCANLINE_TRAILER_HALFWORDS - SCANLINE_FILL_HALFWORDS;
        size_t pos = 0;
        uint32_t x = 0; //pixels emitted so far

        for (auto run : runs) { //RleRun getters aren't const; it's a few ints, copy is cheap
            uint16_t len = run.getLen();
            size_t cost = scanlineRunCost(len);
            if (cost == 0) continue;

            if (pos + cost > runBudget) {
                result.truncated = true;
                if (x < lineWidth) {
                    uint32_t remaining = lineWidth - x;
                    scanlineWriteRun(out, pos, t, static_cast<uint16_t>(remaining), fillColour);
                }
                break;
            }

            scanlineWriteRun(out, pos, t, len, static_cast<uint16_t>(run.getColour()));
            x += len;
        }

        //Black pixel to end the line (as in test_pattern.c), then end-of-line on the right alignment.
        out[pos++] = t.raw1p;
        out[pos++] = 0;
        if (pos % 2 == 1) {
            out[pos++] = t.eolAlign;
        } else {
            out[pos++] = t.eolSkipAlign;
            out[pos++] = 0;
        }

        result.halfwordsWritten = pos;
        result.wordsUsed = pos / 2;
        result.ok = true;
        return result;
    }

} // command

#endif //PICOTEMPLATE_SCANLINEENCODER_H
