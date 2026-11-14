# Scan program fixes

Notes on fixes to the scanvideo / graphicsLib code, and why they were needed.
See also `TODO_MEMORY_BUGS.md`.

## Scanline buffer overflow and end-of-line alignment (TODO 2.2)

Commit `bc20826`, branch `fix-memory-bugs`.

### Background

The scanline code (`CommandProcessor::drawScanline`, `drawSolidColorScanline`,
`graphicsLib::writeSolidColourScanline`) is adapted from the pico-playground example
`scanvideo/test_pattern/test_pattern.c`. That example always writes exactly 32 colour
runs of 3 halfwords each, so its line has a fixed size and always ends on a word
boundary (it even asserts it). Our adaptation writes a variable number of 2- and
3-halfword runs, which broke two assumptions.

### 1. The overflow

The scanline buffer is `PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS` = 180 words
(360 halfwords). Nothing limited how many runs were written into it, and the
`assert` only ran after the writes (and is compiled out in release builds). The
worst case, 400 single-pixel runs, wrote 804 halfwords.

**Fix:** `video/scanProgram/scanPrograms/graphicsLib/command/ScanlineEncoder.h`
(no Pico SDK dependencies, so it can be tested on the host) checks every run
against `data_max * 2` halfwords. If the rest of the runs won't fit, it fills the
remainder of the line with the base colour and ends the line properly.
`CommandProcessor::truncatedScanlines` counts how often that happens; it can't log,
because rendering runs on core 1 with interrupts off.

### 2. The end-of-line token on the wrong alignment

Per `pico-extras/src/common/pico_scanvideo/scanvideo.pio`:

- `end_of_scanline_skip_ALIGN` must start on a word boundary
  (`|| jmp | ignored ||`); it does `out null, 32`, discarding a whole word.
- `end_of_scanline_ALIGN` must start on an odd halfword (`| jmp ||`).

The example always satisfies the first rule. In our code:

- solid-colour lines are 5 halfwords before the token (`COLOR_RUN, colour, n,
  RAW_1P, 0`), so they always ended misaligned;
- text lines ended misaligned whenever their halfword count was odd.

From reading the PIO program, a misaligned `EOL_SKIP_ALIGN` would stall and throw
away the first word of the next line's data. This is an inference from the PIO
source; it hasn't been confirmed on hardware.

**Fix:** pick the token from the actual parity: `EOL_ALIGN` when the count is odd,
`EOL_SKIP_ALIGN, 0` when even. The SDK's own missing-scanline data
(`pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c:298-302`) has exactly
the solid-line shape and uses `EOL_ALIGN`, and `DemoScanProgram` already does the
same. Applied to `drawScanline`, `drawSolidColorScanline` and
`graphicsLib::writeSolidColourScanline`.

### Testing

- Host test under AddressSanitizer, with a small decoder that reads the line data
  the way the PIO does. Cases: one full-width run, odd and even endings,
  400 x 1 px runs, 200 x 2 px runs, and a tiny buffer. No overruns, every line
  ended on the correct alignment, and every line came out at 401 pixels (400 plus
  the final black pixel, same as upstream).
- Video and no-video firmware builds compile.
- **Not yet tested on hardware.**

### What to check on hardware

- Solid-colour screens (menu background, clock) now end with a different token;
  check they look the same.
- Long lines of text: if a line is too long, its right-hand end now shows the
  background colour instead of corrupting memory.
