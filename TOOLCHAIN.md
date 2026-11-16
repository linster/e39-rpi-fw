# Toolchain notes

Machine: Pop!_OS 24.04 (upgraded from 22.04 on 2026-06-28).

## What broke

| When | What | Effect |
|---|---|---|
| 2025-11-11 | `libstdc++-arm-none-eabi-newlib` purged in Synaptic | ARM C++ standard library gone: `fatal error: memory: No such file or directory` |
| 2025-11-12 | `apt autoremove` | Removed the leftover `libstdc++-arm-none-eabi-dev` |
| 2026-06-28 | 22.04 -> 24.04 upgrade | `gcc-arm-none-eabi` went from GCC 10.3 to GCC 13.2, `libnewlib-arm-none-eabi` from 3.3 to 4.4 |

`cmake-build-debug/` was configured with GCC 10.3.1. It's stale and should be recreated.

## Install

```bash
sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib \
                 cmake ninja-build build-essential
# Stop autoremove from taking the C++ library again:
sudo apt-mark manual libstdc++-arm-none-eabi-newlib libstdc++-arm-none-eabi-dev
```

`build-essential` provides the host `g++`, which pico-sdk uses to build `pioasm` and `elf2uf2`.

Check that it works (should print nothing):

```bash
echo '#include <memory>' | arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -x c++ -E - -o /dev/null
```

## Recreate the build directory

The old cache points at GCC 10.3 and CLion 2022.3.1's bundled cmake/ninja in `~/Downloads`.

- CLion: Tools > CMake > Reset Cache and Reload Project.
- Command line: `rm -rf cmake-build-debug && cmake -S . -B cmake-build-debug -G Ninja && ninja -C cmake-build-debug`

## GCC 13 fixes (done on branch `video-crash-fix`)

GCC 13's headers no longer pull in other headers by accident the way GCC 10's did.

1. **pico-sdk `pioasm`** (built with the host `g++` 13) failed with
   `pio_disassembler.h: 'uint16_t' was not declared`. Fixed by updating pico-sdk 1.4.0 -> 1.5.1 (`78208ca`).
   After pulling, run `git submodule update --init --recursive` (1.5.1 adds btstack, mbedtls and a newer tinyusb).
2. **Missing `#include <array>` / `<functional>`** in a few project files. Fixed in `cddaf0a`.

The stray `proto` -> `proto_generated` rename edits inside `pico-sdk/lib/lwip` and `pico-sdk/lib/tinyusb`
were stashed (`git -C pico-sdk/lib/lwip stash list`) before the SDK update. They look like an accidental
IDE refactor and are safe to drop.

## Video build (fixed on branch `video-crash-fix`)

Video is switched by `set(CMAKE_HAS_VIDEO_SUPPORT 0|1)` in `CMakeLists.txt`. The video build had been broken
since `ee4aeaf` (2024-02-22, "video support compiled out"):

1. `PimoroniFactory` still included `factory/provider/DmaManagerProvider.h`, which `ee4aeaf` deleted.
   The provider was never used, so it was removed from `PimoroniFactory` (as `NoVideoFactory` already does).
2. `video/CMakeHasVideoFlags.in.h` used `@CMAKE_HAS_VIDEO_VALUE@`, which is never set, so the header became
   `#define CMAKE_HAS_VIDEO_SUPPORT` (empty) and every `#if CMAKE_HAS_VIDEO_SUPPORT == ...` failed to compile.
   It now defines `CMAKE_HAS_VIDEO_SUPPORT` as `CMAKE_VIDEO_SUPPORT_HAS_VIDEO` when video is on, and leaves it
   undefined when off.

Both `CMAKE_HAS_VIDEO_SUPPORT 0` and `1` build and link with GCC 13.2.1 + pico-sdk 1.5.1 (checked 2026-11-13).
