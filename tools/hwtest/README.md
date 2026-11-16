# Bench test tools (debug probe + IBus dongle)

Python 3 tools used to test the firmware on the bench: a Raspberry Pi Debug Probe (SWD) on the
Pico, and a USB IBus interface (CP2102 + transceiver, `/dev/ttyUSB0`) on the bus. Only the standard
library is needed: serial ports go through `termios` (`serialport.py`, 9600 8E1), SWD through
openocd's TCL port.

## Setup

```bash
# Flash a build (one-shot), then leave an openocd server running for the tools:
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" \
        -c "program cmake-build-debug/<build>/PicoTemplate.elf verify reset exit"
openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000" \
        -c "tcl_port 6666" -c "gdb_port 3333" -c "telnet_port 4444" &
```

Use openocd 0.11+ (the Raspberry Pi fork has `target/rp2040.cfg`). openocd treats the two cores as
one SMP target: when one core hits a breakpoint (e.g. the SDK's HardFault handler is a `bkpt`), it
halts the other too.

## Tools

| Tool | What it does | Halts the cores? |
|---|---|---|
| `soak_watch.py SECONDS ELF` | Watches for a HardFault, lockup, halt or watchdog reset; every 60 s prints where each core spends its time (`DWT_PCSR` PC sampling). Exit code 1 on a fault. | No |
| `vidcheck.py` | Is scanvideo outputting lines? Histogram of the scanline state machine's PC, plus colour/sync pin activity and the video switch position. A stuck SM shows as `{1: 1500}` (parked at `wait 1 irq 4`). | No |
| `fault_frame.py ELF CORE` | Dumps the exception frame of a core sitting in `isr_hardfault`. Prefer `gdb-multiarch` + `thread apply all bt` for a full backtrace. | Yes (that core) |
| `stack_paint.py paint` / `read` | Stack high-water marks: `paint` fills unused stack below each SP with a pattern, `read` reports how far down it was overwritten. Set `ELF=...` so it finds `core1Stack`. | Briefly (paint) |
| `flashcfg.py` | Prints the stored config frame (`E39C` + length + payload) from the persistent flash sector. | No |
| `safe_loadtest.py --elf ELF` | Safe IBus load test (below). Reads the DMA manager's drop counters by symbol. | No |
| `menu_test.py --sends-real-bmbt-and-ike-messages visual\|stress` | Opens and drives the menu with BMBT knob/telephone messages and IKE ignition messages. **Sends real module messages**; see its docstring. | No |
| `swd_watch.py` | Library used by the others (openocd TCL client, symbol lookup). | — |

## Safety rules for transmitting on a bus with real modules

These are from incidents on the bench:

1. **Never send random or fuzzed packets to real addresses, broadcasts (`FF`, `BF`) or from the
   diagnostic address (`3F`).** A random flood made the signal lights flicker, put garbage on the
   IKE's text display, rolled the Pi's screen (video format messages) and left the ABS, parking
   brake and traction control lights on after a power cycle.
2. **Unused addresses are not enough: leave the bus idle.** Flooding `14 -> 12` (no module uses
   those addresses) back to back at ~840 B/s also made the signal lights flicker. On a saturated
   9600-baud single-wire bus, real modules never see the ~1.5 ms of silence they wait for before
   transmitting, so their status messages are starved. `safe_loadtest.py` waits for 1.5 ms of
   silence before each packet and leaves at least 10 ms (default 20 ms) between its packets.
3. List exactly what will be sent (source, destination, data) before running anything that
   transmits.

## Load test results (bench, `menu-core0` firmware)

Saturation run (`14 -> 12` back to back, before rule 2 was known): 73 s, 56 KB at ~770 B/s.

- 0 received bytes dropped (`rxBytesDropped`), 0 oversized, 0 malformed outgoing packets.
- Pi -> Pico ConfigPush probes: 2/2 handled and saved to flash under full load.
- Video kept running; no fault on either core.

So the Pico keeps up with a saturated bus. It was stopped early only because it starved the
bench's real modules. `safe_loadtest.py` (the paced version) hasn't been run on the bench yet.

## Known issue found while testing

The Pico's own transmitter (`SingleCoreDmaManager::writeOutgoingQ_toUart()`) writes packets
immediately with `uart_write_blocking()`: no wait for an idle bus, no spacing, no collision
detection. On a busy bus it can collide with real modules. It should wait for ~1.5 ms of silence
before sending and space its own packets.

References for the timing: community I/K-bus implementations wait for ~1.5 ms of bus silence before
transmitting, and space their own messages by 7-10 ms
(e.g. [BlueBus IBus notes](https://www.mintlify.com/tedsalmon/BlueBus/development/ibus-protocol),
[ESP32 I/K-bus board architecture](https://git.supported.systems/warehack.ing/i-k-bus-board/raw/branch/main/docs/architecture.md)).
No official BMW or TH3122 figure was found.
