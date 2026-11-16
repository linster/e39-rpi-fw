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

## Font table out-of-bounds read (TODO 2.3)

Branch `fix-memory-bugs`.

### The bug

`TextCommand::getRleRunsForCharacter` (`graphicsLib/command/TextCommand.cpp`) meant to
draw the copyright glyph (`font8x8_ext_latin[9]`, U+00A9) for any non-ASCII byte, but
the `else` was missing:

```cpp
if (ord > 127 || ord < 0) {
    bitmap = font8x8_ext_latin[9];
}
bitmap = font8x8_basic[ord];   // always runs
```

`font8x8_basic` has 128 entries, so every byte >= 128 read past the end of the table
(up to 1 KB past it). `char` is unsigned on ARM, so on the Pico `ord` is 128..255;
on the host it's signed, so `ord` is -128..-1 and the read goes before the table.

### Fix

- Added the `else`, so only 0..127 index `font8x8_basic`.
- Convert with `(unsigned char)c`, so the Pico and host unit tests behave the same.

### Testing

- Host test under AddressSanitizer renders each byte value 0..255 and checks the pixel
  count matches the expected glyph: the basic table for 0..127, the copyright glyph for
  128..255. The new code passes; the old code fails at byte 128 (wrong glyph, read from
  `font8x8_basic[-128]` on the host).
- Video firmware build compiles.
- **Not yet tested on hardware.**

### Known limitation (unchanged)

Strings are probably UTF-8, so a multi-byte character draws one glyph per byte: a
UTF-8 "©" (`C2 A9`) draws two copyright glyphs.

## `addRleRuns` leak and runs that never rendered (TODO 2.4)

Branch `fix-memory-bugs`.

### The bug

`graphicsLib::contributeRleRuns` and `drawTextSpecialCharacter` call
`CommandProcessor::addRleRuns`, which pushed a `RleRunContainerCommand` into
`commandsToProcess`. Nothing has read `commandsToProcess` since commit `123f0b3`
("Seems to be more stable, but video still dies after pressing things"): that commit
changed `addCommand` to rasterize straight into `rleRunsToProcess` and made
`computeFrame` read only that, but `addRleRuns` was missed. Its `clear()` was later
commented out. So every call leaked a command (and a copy of its runs), and the runs
never appeared on screen.

Affected callers:

- `BootsplashScanProgram::onScanProgramStart_Goose`: the whole goose image.
- The LinsterOS bootsplash: the copyright symbol before "Stefan Martynkiw 2019-2024".

Separately, the `RleRunContainerCommand` constructor read `runs.end()->first`
(dereferencing the end iterator: undefined behaviour on every call), assigned
`topLine` twice, and never set `bottomLine`. An empty map would also have made
`begin()->first` invalid.

### Fix

- `addRleRuns` now calls `addCommand(std::make_unique<RleRunContainerCommand>(runs))`,
  so the runs are rasterized into `rleRunsToProcess` in call order (drawing order is
  preserved, as the container's comment intends) and freed by `computeFrame`.
- Removed the unused `commandsToProcess` member.
- Constructor: `topLine = begin()->first`, `bottomLine = rbegin()->first`, both 0 for
  an empty map.

### Testing

- Host test under AddressSanitizer of the container constructor (normal and empty
  maps).
- Video firmware build compiles.
- **Not yet tested on hardware.**

### What to check on hardware

- The goose now appears on the Goose bootsplash, and the copyright symbol appears on
  the LinsterOS bootsplash. Neither could render before, so check their position and
  colour look right.

---

# Video fixes from hardware testing (branch `video-fixes`)

Found while testing PR #5 and PR #6 on the bench with a debug probe. How things were
observed: openocd + SWD, mostly without halting the cores (`DWT_PCSR` PC sampling, `DHCSR`
halt/lockup bits, PIO registers and `GPIO_IN` read while running), plus a host replay of
the bootsplash through the real `CommandProcessor` for the rendering bug. The tools are in
`cmake-build-debug/hwtest/` (not committed).

## Core 1 HardFault: scan program deleted while in use (`8d06b4b`)

### The bug

Core 1 crashed with stacked `pc = 0x00000000`, `lr = BaseScanProgram::callRender()+0x1c`,
and `this` holding the vtable of the abstract `BaseScanProgram`: `render()` was called on
an object that was being destroyed.

`ScanProgramManager::getScanProgramPtr()` returned a `std::shared_ptr` copy, and it's called
constantly on both cores: every main-loop iteration on core 0, every scanline on core 1. On
this toolchain `shared_ptr` reference counts aren't atomic (`_Lock_policy` `_S_single`; all
780 `shared_ptr` symbols in the binary). Concurrent copies lost updates until the count hit
zero and the scan program was deleted.

### Fix

Return a raw pointer. `ScanProgramManager` owns the scan programs for the life of the
program, so nothing needs reference counting there.

## Same race when core 1 logs (`8d5ee04`)

With `CAR_WITH_PI`, every log line goes through `IbusLoggerOutput`, which reached the DMA
manager through a `std::function` returning a `shared_ptr` copy and then
`BaseOutputWriter::getDmaManager()` (another copy). Core 1 logs too (scan program
start/stop, boot), racing core 0's output writers on the same count.

The accessor now returns a raw pointer, and `BaseOutputWriter` writes through a new
`getDmaManagerForWrite()`, which `IbusLoggerOutput` overrides.

Rule of thumb for this codebase: **never copy a `shared_ptr` that the other core also
copies.** Pass raw pointers or references across cores.

## Blank screen a few frames after boot (`52fdaed`)

### Symptom

Video showed for a few frames after boot (partial "Linster OS"), then the screen went blank.
On the pins: sync (GP6/GP7) still running, colour pins frozen, and PIO0 SM0 (the scanline
state machine) parked at `wait 1 irq 4`.

### The bug

`BaseScanProgram::callRender()` ran `render()` inside `renderCs`, a critical section that
disables interrupts on core 1 for the whole line. scanvideo's per-line interrupts
(`PIO0_IRQ_0`, `DMA_IRQ_0`) also run on core 1 and must launch each line's DMA within about
4.5 us. Once a real frame was computed, rendering a line took long enough that they ran late
on every line, and the scanline state machine never started a line again. Nothing else ever
took `renderCs`; `CommandProcessor` has its own lock for the state it shares with core 0.

### Fix

Removed the critical section.

## Text erased the lines it was drawn on (`d882e2b`)

### Symptom

Header and background fine, but the bottom bar's text ("e39-Rpi", "(c) Stefan Martynkiw
2019-2024") missing or corrupted.

### The bugs

A host replay of `drawLinsterOs()` through the real `CommandProcessor` and scanline encoder
showed:

1. `computeFrame()` rebuilt each touched line from **only the newly added runs**. In
   immediate mode (one `computeFrame()` per `addCommand()`), the last command to touch a line
   replaced everything on it: text erased the background behind it, the white "Linster OS"
   erased its drop shadow, and the (c) glyph (which only started rendering with the TODO 2.4
   fix) erased the whole "Stefan Martynkiw 2019-2024" line.
2. `mergeRuns()` never emitted the final run, so each line ended at its last colour change.
3. `mergeRuns()` painted `len + 1` pixels per run.

### Fix

`computeFrame()` starts each line from its existing runs and paints the new ones on top;
`mergeRuns()` paints exactly `[startX, startX + len)` and always emits the final run. In the
replay every line is the full 400 px and the largest needs 244 of 360 halfwords (no
truncation). Confirmed on the screen.

## Scanline state machine wedges at boot (`6617402`, workaround)

### Symptom

On some boots (seen 0 of 4, then 2 of 3 boots fine), the screen goes blank: SM0 sits at
`wait 1 irq 4` forever although the timing state machine keeps raising IRQ 4 and SM0's TX
FIFO holds the next line (`EXEC_STALLED` = 0, so it's the program's own instruction). Core 1
then blocks forever in `scanvideo_begin_scanline_generation()`. A manual `SM_RESTART` over
SWD clears it and video stays up.

### What it isn't

- Not the rendering changes: `52fdaed` (before them) wedged the same way.
- Not the flash save: wedged with the persistent config sector erased and never written.

### Workaround

`ScanProgramManager::onCpu0Loop()` (core 0, since core 1 is blocked when this happens)
restarts SM0 if it's seen at that wait on at least 200 consecutive checks spanning more than
50 ms; a healthy line waits there for well under one line (about 65 us). It counts recoveries
in `ScanProgramManager::scanlineSmRecoveries` and logs a warning. On hardware: video up on 6 of
6 boots, one of which needed a recovery.

**Root cause still unknown.** It looks like a boot-time race in the scanvideo start-up.

### Testing (all of `video-fixes`)

- 10-minute soak of `52fdaed` with the non-halting watcher: no fault, video still running.
- Watchdog build: video up on 6 of 6 boots; the bootsplash renders correctly on the screen.
- Video and no-video builds compile.
