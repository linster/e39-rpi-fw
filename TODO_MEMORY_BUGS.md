# Memory-safety and related bugs — TODO

Found in a review on 2026-11-13 (branch `video-crash-fix` @ `802ec59`).
Items marked **[ASan-confirmed]** were reproduced with an AddressSanitizer
harness on the host. Suggested order: fix the flash read first (it corrupts
the heap on every boot), then the cross-core map race (most likely video crash).

## 1. Configuration store (why config can't be loaded/stored)

- [x] **1.1 Flash read overflows the heap and never sees the data.**
  `configuration/store/flash/FlashConfigurationStore.cpp:87-90` — `flashBytes`
  is an empty vector and the loop writes 4096 bytes into it via `operator[]`
  (4 KB heap overflow, every boot via `canReadConfiguration()`). The vector's
  size stays 0, so `flashString` is empty, the decoded hash is empty, and
  `canReadConfiguration()` always returns false.
  Fix: `std::vector<uint8_t> flashBytes(flashPtr, flashPtr + 4096);` (or decode
  straight from `flashPtr`).
  **Done:** also needed a stored length — nanopb's `pb_decode()` rejects the
  zero padding after the message ("zero tag"), so the old format could never
  decode. Block is now framed as magic `E39C` + 2-byte length + payload
  (`FlashConfigurationFraming.h`); decode reads only the payload. Host
  round-trip test passes under ASan; firmware builds.
- [x] **1.2 Defaults overwrite flash on every boot.** Because 1.1 always fails,
  `ConfigurationManager.cpp:34-37` saves the default config to flash, so
  anything the Pi pushed is lost on the next power cycle.
  **Resolved by 1.1 + 1.4:** a stored config now loads, so defaults are only
  written when flash has no valid frame (first boot / blank flash).
- [x] **1.3 Flash erase/program breaks SDK rules.**
  - `flash_range_erase(flash_offset, 1)` (line 126): count must be a multiple
    of `FLASH_SECTOR_SIZE` (4096).
  - Interrupts aren't disabled during erase/program; the UART RX ISR running
    from flash will hard-fault or hang. Use `save_and_disable_interrupts()` /
    `restore_interrupts()`, or `flash_safe_execute()`.
  - Video build: `multicore_lockout_start_blocking()` requires core 1 to have
    called `multicore_lockout_victim_init()`; nothing does (commented out in
    `ApplicationContainerNoVideo.cpp:58`). The first save also happens in
    `onMain()` before core 1 is launched. Either way it blocks forever.
  **Done:** erase a full `FLASH_SECTOR_SIZE`; interrupts disabled around
  erase/program; size checked (non-empty, <= 4096, multiple of 256). Core 1
  calls `multicore_lockout_victim_init()` in `core1_entry` right after the FIFO
  pop. The store locks out core 1 only if it registered (500 ms timeout, then
  refuse to write); otherwise core 1 is not running our code (no-video build,
  or still in bootrom during `onMain()`), so interrupts-off is enough. No
  longer depends on the video `#if`, which removes 1.6's impact here.
  Not yet tested on hardware; video build not compiled (current config is
  no-video).
- [x] **1.4 Load requires a commit hash.** A config is only "loaded" if
  `rpiFwGitCommitHash` is non-empty (line 101). Only
  `PimoroniDefaultConfigurationProvider` sets it (`"lalalalalalala"`), so a
  Pi-pushed config without a hash never loads.
  **Done:** `decodeConfiguration()` now treats frame + successful decode as
  valid; the hash is no longer required.
- [x] **1.5 Large configs wrap silently.** `encodeConfiguration` uses a
  `uint8_t bytesIndex` into a 256-byte vector; encodings > 256 bytes wrap and
  overwrite the start instead of failing.
  **Done with 1.1:** framing refuses payloads > 4090 bytes and logs a warning.
- [ ] **1.6 Video flag checked two different ways.** `main.cpp` uses
  `#ifdef CMAKE_HAS_VIDEO_SUPPORT`; the flash store uses
  `#if CMAKE_HAS_VIDEO_SUPPORT == CMAKE_VIDEO_SUPPORT_HAS_VIDEO`. The
  checked-in `video/CMakeHasVideoFlags.h` defines the macro as 0 (no video),
  which `#ifdef` treats as true — `main` would launch core 1 while the flash
  store skips the lockout. Verify which header wins.

## 2. Video (likely causes of the video crash)

- [ ] **2.1 Cross-core data race → use-after-free** (`CommandProcessor.cpp`).
  Core 0 `clear()`s and reassigns `rleRunsForLine` (`std::map`) in
  `clearFrame()`, `computeFrame()`, `clearScanlines()` while core 1 reads it in
  `render_computed()` (including `rleRunsForLine[line_num]`, which can insert,
  and copies the vector). The only guard is a plain `bool isFrameComputed`,
  checked once then trusted; mutex calls are commented out.
  Fix: real lock around the map, or double-buffer (build a new map on core 0,
  swap a pointer atomically).
- [ ] **2.2 Scanline buffer overflow** (`CommandProcessor::drawScanline`).
  Buffer is `PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS` = 180 words (360
  halfwords); each run writes 2–3 halfwords and a 400 px line can produce
  hundreds of runs (e.g. a line of text). The `assert` runs after the writes
  and is compiled out in release. Check remaining space before each run.
- [ ] **2.3 Font table OOB read** (`TextCommand.cpp:51-57`). Missing `else`:
  for characters >= 128, `font8x8_basic[ord]` reads past the 128-entry table.
  `char` is unsigned on ARM, so any non-ASCII byte triggers it.

## 3. IBus receive / transmit

- [ ] **3.1 `Packetizer` constructor** (`Packetizer.cpp:77-78`): builds a
  255-byte vector then `clear()`s it, so size is 0 until the first `reset()`;
  every `packetBytes[position] = byte` before that is out of bounds.
  **[ASan-confirmed]** (`Assertion '__n < this->size()' failed`).
- [ ] **3.2 `Packetizer` with length byte 254/255:** `uint8_t position`
  indexing a 255-byte buffer. Length 254 writes `packetBytes[255]`; with 255 the
  end-of-packet and reset checks can never be true and `position` wraps
  forever. One bad byte on the bus triggers it.
- [ ] **3.3 `Packetizer::recycle()`** (line 180): loop tests
  `position < size()` instead of `i` → reads past the end forever. Unused today.
- [ ] **3.4 `IbusPacket(std::array…)` constructors** (`IbusPacket.cpp:115-117,
  135-137`):
  - Length 253–255 reads past the 255-byte array. **[ASan-confirmed]**
    (heap-buffer-overflow).
  - CRC read from `raw[len+2]` instead of `raw[len+1]`, so every correct packet
    from `readIncomingQ_toLogic` reports invalid. **[confirmed]**
    (`valid=0 given=0 actual=0x72`). Nothing calls `isPacketValid()` yet.
  - `len > 255` is always false for `uint8_t`.
- [ ] **3.5 `writePacketToQ`** (`SingleCoreDmaManager.cpp:607-610`): copies the
  packet into a 255-byte stack array with a `uint8_t` index; packets >= 256
  bytes write `[255]` past the end, then wrap. Pico→Pi protobuf messages can be
  up to 255 + 4 bytes (logs are truncated to 64 chars; config messages are the
  risk).
- [ ] **3.6 `writeOutgoingQ_toUart`** (line 436): sends `buf[1] + 2` bytes from
  a 255-byte stack array → reads up to 2 bytes past the end for length 254/255.

## 4. Lower severity (logic, not memory)

- [ ] **4.1** `BaseObserver::decodePiToPicoMessage` builds input via
  `std::make_unique<std::string>(inputString.c_str())`, which truncates the
  protobuf at the first `0x00`. Pass `inputString` itself.
- [x] **4.2** `IgnitionStatusRequestWriter.cpp:21`: `std::vector<uint8_t>(1)`
  then `push_back(0x10)` sends `{0x00, 0x10}`; probably want `{0x10}`.
  **Done:** payload is now `std::vector<uint8_t>{0x10}`.

## 5. Unit-test repo (`~/e39-rpi-fw-unittest`)

- [ ] Update the submodule (pinned at old `f638c6c`) to current firmware.
- [ ] Build with `-fsanitize=address,undefined -D_GLIBCXX_ASSERTIONS`.
- [ ] Tests: length bytes 0/253/254/255 on `Packetizer` and `IbusPacket`;
  config round-trip through a fake flash buffer; `drawScanline` with a line of
  alternating pixels.
