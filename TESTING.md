# In-car test plan for the video crash fixes

Background and root-cause analysis: `video/Readme.md`. Build setup: `TOOLCHAIN.md`.

## Test branches

All test branches are built from `video-crash-fix` (pico-sdk 1.5.1 + GCC 13 build fixes) with one extra
commit that sets `CMAKE_HAS_VIDEO_SUPPORT 1` and `CMAKE_BUS_TOPOLOGY_CAR_WITH_PI`. They differ only in
which crash fixes are included:

| Branch | Fix 1+2: ISR overflow + drain queues | Fix 3: no vblank gate | Fix 4: exact pixel clock | Expected |
|---|---|---|---|---|
| `test/video-baseline` | no | no | no | Crashes on a chatty bus (confirms the repro) |
| `test/video-fix-1-2` | yes | no | no | Should stop the crash |
| `test/video-fix-1-3` | yes | yes | no | Stable, and IBUS keeps up better |
| `test/video-fix-1-4` | yes | yes | yes | Same, with 15.38 kHz line rate (check BMBT picture) |

## Building and flashing

```bash
git checkout test/video-fix-1-2          # or another branch from the table
git submodule update --init --recursive
rm -rf build && cmake -S . -B build -G Ninja && ninja -C build
# Hold BOOTSEL, plug in the Pico, then copy build/PicoTemplate.uf2 to the RPI-RP2 drive.
```

Prebuilt `.uf2` files for each branch were also left in `~/e39-rpi-fw-test-uf2/`.

## What to check

1. Run each build with video on (menu / clock scan program) while the IBUS is busy.
2. Does it hard-fault or reboot (watchdog)? How long until it does?
3. Fix branches only: watch the `Cpu0 Queue Health` log line. `rxBytesDropped` should stay at or near 0.
   If it climbs, the main loop isn't keeping up with the bus, but it should no longer crash.
4. Fix 4 only: does the BMBT still lock onto the picture (no rolling, position OK)?
