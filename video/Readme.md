# Video + IBUS hard-fault investigation

## Symptom
With video enabled, the Pico runs fine while the IBUS is quiet. When the bus gets
chatty, it hard-faults (looks like missed ISRs / DMA), and everything breaks.
Without video, the same firmware is stable.

The original theory (see the comment above `CMAKE_VIDEO_SUPPORT_NO_VIDEO` in
`CMakeLists.txt`) was that the pixel clock hack causes DMA starvation. The evidence
below points somewhere else.

## The pixel clock hacks

1. `pico-extras/src/rp2_common/pico_scanvideo_dpi/scanvideo.c` (submodule commit
   `84c4cc7`, "Stefan YOLO on pixel clock constraint") comments out the
   `panic("System clock must be an integer multiple of the requested pixel clock")`.
   scanvideo then computes `video_clock_down_times_2 = 125000000 / 7867500 = 15`
   (truncated from 15.888), so the PIO divider is 7.5 and the real pixel clock is
   **8.333 MHz**, not 7.8675 MHz (~5.9% fast).
2. `video/scanProgram/ScanProgramManager.cpp` `cpu1setup()` has notes and
   `measureFreqs()` calls about retuning clk_sys. Nothing is changed; clk_sys stays at 125 MHz.

## Timing vs. the known-working Pi timing

Reference: `e39-rpi/systemImage/builder/config_txt/config.400x234.menu.ntsc.txt`
`hdmi_timings=400 0 20 29 59 234 0 7 3 19 0 0 0 50 1 7867500 3`

Pico: `video/scanProgram/scanPrograms/BaseScanProgram.h`

|                          | Pi (works)       | Pico as written | Pico actual (pre-fix) |
|--------------------------|------------------|-----------------|-----------------------|
| H active/front/sync/back | 400/20/29/59     | same            | same                  |
| H total                  | 508              | 508             | 508                   |
| V active/front/sync/back | 234/7/3/19       | same            | same                  |
| V total                  | 263              | 263             | 263                   |
| Pixel clock              | 7.8675 MHz       | 7.8675 MHz      | 8.333 MHz             |
| Line rate                | 15.49 kHz        |                 | 16.40 kHz             |
| Frame rate               | ~58.9 Hz         |                 | ~62.4 Hz              |
| Sync polarity flags      | 0 / 0            | h=1, v=1        |                       |
| Interlaced               | yes              | no (scanvideo is progressive) |         |

## Why the clock hack is probably not the crash
- scanvideo's PIO/DMA IRQs are set up from `cpu1setup()`, so they run on core 1.
  The UART RX IRQs are set up from `cpu0setup()`, so they run on core 0. NVICs are
  per-core, so UART traffic can't preempt or delay the video IRQs.
- 16.4 kHz is about half the scanline IRQ rate of scanvideo's stock 640x480 mode
  (31.5 kHz). Running 6% fast doesn't stress it.

## Root cause: use-after-free in the UART RX ISR
`ibus/dma/SingleCoreDmaManager.cpp` `handleRxInterruptServiceRoutine()` called
`queue_free(toQ)` when the byte queue was full. In this SDK, `queue_free()` just calls
`free(q->data)`. The queue stays in use, so every later RX byte is written into freed
heap memory. That corrupts the heap and causes hard faults that can show up anywhere,
including the video IRQ/DMA paths.

The queue only fills with video enabled because `onCpu0Loop()` returns early unless
`scanvideo_in_vblank()` (29 of 263 lines, ~11% duty). Even then it moved only **one
byte per loop** into the packetizer. A 9600 8E1 bus can deliver ~870 B/s per UART, so
a chatty bus overflows the 2048-byte queue. Without video there's no vblank gate and
the queue keeps up.

## Fixes (branch `video-crash-fix`, one commit each so they can be tested separately)
1. **ISR overflow**: drop the byte and count it in `rxBytesDropped` (now included in the
   queue health status log). Never `queue_free()` a live queue.
2. **Drain fully**: empty the UART byte queues each loop, and dispatch all queued
   incoming packets (draining the bytes can complete several packets per loop, and
   `fromCarQ`/`fromPiQ` only hold 10).
3. **Remove the vblank gate** in `SingleCoreDmaManager::onCpu0Loop()`. Video is on
   core 1, so there's no reason to make core 0's message pump wait for it.
4. **Exact pixel clock**: 7.8675 MHz can't be divided exactly from the 12 MHz crystal
   at any usable clk_sys. Use `clock_freq = 7812500` (125 MHz / 16, exact), which is
   0.7% from the Pi's clock. Line rate becomes 15.38 kHz (vs 16.40 kHz with the hack).
   The `panic` stays commented out in the pico-extras submodule, but with an exact
   divider that branch is no longer reached.
