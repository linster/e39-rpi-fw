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

## GCC 13 fixes still needed

GCC 13's headers no longer pull in other headers by accident the way GCC 10's did.
With the library installed, these are the only remaining failures (checked 2026-11-13):

1. **pico-sdk 1.4.0 `pioasm`** (built with the host `g++` 13):
   `pio_disassembler.h: 'uint16_t' was not declared`. Upstream fixed this in pico-sdk 1.5.1
   by adding `#include <cstdint>`. Either upgrade the submodule, or patch it locally
   (local change inside the submodule, so it has to be redone after a fresh clone):
   ```bash
   sed -i 's|#include <string>|#include <string>\n#include <cstdint>|' pico-sdk/tools/pioasm/pio_disassembler.h
   ```
2. **Missing `#include <array>`**: `ibus/data/IbusPacket.h`, `ibus/dma/SingleCoreDmaManager.h`
3. **Missing `#include <functional>`** (video build only): `video/screenManager/ScreenManager.h`,
   `video/scanProgram/scanPrograms/menu/MenuScanProgram.cpp`

The alternative is Arm's GCC 10.3-2021.10 tarball (it bundles newlib and libstdc++) via
`PICO_TOOLCHAIN_PATH`. That avoids items 2 and 3, but not item 1, because `pioasm` uses the host compiler.

## Video build is broken regardless of toolchain

With `set(CMAKE_HAS_VIDEO_SUPPORT 1)` in `CMakeLists.txt`, the build also fails on:

1. `factory/pimoroniFactory/PimoroniFactory.h` includes `factory/provider/DmaManagerProvider.h`,
   which was deleted in `c67bc27` (2024-01-29).
2. `video/CMakeHasVideoFlags.in.h` uses `@CMAKE_HAS_VIDEO_VALUE@`, but `set(CMAKE_HAS_VIDEO_VALUE ...)`
   is commented out in `CMakeLists.txt`. The header then becomes `#define CMAKE_HAS_VIDEO_SUPPORT` (empty),
   and every `#if CMAKE_HAS_VIDEO_SUPPORT == ...` fails with "operator '==' has no left operand"
   (`configuration/store/flash/FlashConfigurationStore.cpp`, `ibus/dma/SingleCoreDmaManager.h`).
   With video off it's `#undef`, so those `#if`s quietly evaluate as "no video".
