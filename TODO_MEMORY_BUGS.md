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
- [x] **1.6 Video flag checked two different ways.** `main.cpp` uses
  `#ifdef CMAKE_HAS_VIDEO_SUPPORT`; `SingleCoreDmaManager.h` (and formerly the
  flash store) uses `#if CMAKE_HAS_VIDEO_SUPPORT == CMAKE_VIDEO_SUPPORT_HAS_VIDEO`.
  **Already fixed before this review, in `48ff72f` ("Fix the video build"):** the
  template now uses `#cmakedefine`, so the macro is defined (as `HAS_VIDEO`) only
  in the video build and both styles agree. The review's finding was wrong about
  the cause: there is no checked-in header; it saw a stale generated header in
  `cmake-build-debug/` from before `48ff72f`, which still defined the macro as 0.
  That build dir regenerated it on the first build this session.
  Verified: no-video ELF links `NoVideoFactory` only (no core 1 launch, no
  scanvideo); video ELF links `PimoroniFactory`, `multicore_launch_core1` and
  scanvideo. No code change. (1.3 also removed the flash store's dependency on
  the flag.)

## 2. Video (likely causes of the video crash)

- [x] **2.1 Cross-core data race → use-after-free** (`CommandProcessor.cpp`).
  Core 0 `clear()`s and reassigns `rleRunsForLine` (`std::map`) in
  `clearFrame()`, `computeFrame()`, `clearScanlines()` while core 1 reads it in
  `render_computed()` (including `rleRunsForLine[line_num]`, which can insert,
  and copies the vector). The only guard is a plain `bool isFrameComputed`,
  checked once then trusted; mutex calls are commented out.
  Fix: real lock around the map, or double-buffer (build a new map on core 0,
  swap a pointer atomically).
  **Done:** mutators run on both cores (scan program start/stop on core 1,
  IBus menu redraws on core 0), so two locks: `pendingMutex` serializes all
  mutators; short `renderMutex` guards only what render reads
  (`rleRunsForLine`, `isFrameComputed`, `baseColour`). Merging happens outside
  `renderMutex`; results are swapped in one scanline at a time and old data is
  freed after release. `render_computed` uses `find()` (no insert) and no
  longer copies the vector (no allocation inside `renderCs`). Video and
  no-video builds compile; not yet tested on hardware.
- [x] **2.2 Scanline buffer overflow** (`CommandProcessor::drawScanline`).
  Buffer is `PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS` = 180 words (360
  halfwords); each run writes 2–3 halfwords and a 400 px line can produce
  hundreds of runs (e.g. a line of text). The `assert` runs after the writes
  and is compiled out in release. Check remaining space before each run.
  **Done:** new `ScanlineEncoder.h` (host-testable) budgets every run against
  `data_max * 2` halfwords; if runs don't fit, the rest of the line is one run of
  `baseColour` and the line is ended properly (`truncatedScanlines` counts it).
  Old worst case (400 x 1 px runs) wrote 804 halfwords into a 360-halfword buffer.
  Also fixed end-of-line alignment: the code is adapted from pico-playground
  `test_pattern.c`, which is always word-aligned before `EOL_SKIP_ALIGN`; ours
  wasn't (variable 2/3-halfword runs; solid lines are 5 halfwords). Per
  scanvideo.pio, `EOL_SKIP_ALIGN` must start on a word boundary, so odd counts
  now use `EOL_ALIGN` (the shape scanvideo.c's missing-scanline data and
  `DemoScanProgram` already use). Applied to `drawScanline`,
  `drawSolidColorScanline` and `graphicsLib::writeSolidColourScanline`.
  Host test under ASan passes (bounds, alignment, 401 px/line); both builds
  compile; not yet tested on hardware.
- [x] **2.3 Font table OOB read** (`TextCommand.cpp:51-57`). Missing `else`:
  for characters >= 128, `font8x8_basic[ord]` reads past the 128-entry table.
  `char` is unsigned on ARM, so any non-ASCII byte triggers it.
  **Done:** added the `else`, and read the byte as `unsigned char` so ARM and
  host behave the same. Non-ASCII bytes draw the copyright glyph as intended.
  Host test of all 256 byte values passes under ASan; video build compiles.
  See `SCANPROGRAM_FIXES.md`.
- [x] **2.4 `commandsToProcess` grows forever** (`CommandProcessor::addRleRuns`).
  `graphicsLib::drawTextSpecialCharacter` → `addRleRuns` pushes a
  `RleRunContainerCommand` into `commandsToProcess`, but nothing ever processes
  or clears it (the `clear()` is commented out). Each call leaks, and the
  special character never renders. Likely fix: merge the runs into
  `rleRunsToProcess` like `addCommand` does.
  **Done:** leftover from `123f0b3`, which moved `addCommand` to rasterize
  straight into `rleRunsToProcess` but missed `addRleRuns`. `addRleRuns` now goes
  through `addCommand`; `commandsToProcess` removed. Also fixed the
  `RleRunContainerCommand` constructor (dereferenced `end()`, never set
  `bottomLine`). Goose bootsplash and the bootsplash copyright symbol will now
  actually render. See `SCANPROGRAM_FIXES.md`.

## 3. IBus receive / transmit

- [x] **3.1 `Packetizer` constructor** (`Packetizer.cpp:77-78`): builds a
  255-byte vector then `clear()`s it, so size is 0 until the first `reset()`;
  every `packetBytes[position] = byte` before that is out of bounds.
  **[ASan-confirmed]** (`Assertion '__n < this->size()' failed`).
  **Done:** constructor now sizes the buffer to 255 zeros (no `clear()`), like
  `reset()`. The unit-test repo's `Packetizer_Test.cpp`, built against current
  code with ASan + `_GLIBCXX_ASSERTIONS`, aborted on its first `addByte`; now
  7/7 non-recycle tests pass. `RecycleNoInterleave` still aborts: that's 3.3.
- [x] **3.2 `Packetizer` with length byte 254/255:** `uint8_t position`
  indexing a 255-byte buffer. Length 254 writes `packetBytes[255]`; with 255 the
  end-of-packet and reset checks can never be true and `position` wraps
  forever. One bad byte on the bus triggers it.
  **Done:** length bytes > 253 (`Packetizer::MAX_LENGTH_FIELD`; total packet
  must fit the 255-byte buffers/queues downstream) are rejected with a reset.
  A checksum mismatch now resets immediately; before, it kept accepting bytes
  until `position > len + 2`, which swallowed the start of the next packet and,
  for len 253, wrote `packetBytes[255]`. Added a final bounds check before the
  write. New host tests (len 253/254/255, bad checksum + recovery, 200k random
  bytes) pass under ASan + `_GLIBCXX_ASSERTIONS`; old code overflows in 3 of
  them and loses the following valid packet in 3. No-video build compiles.
- [x] **3.3 `Packetizer::recycle()`** (line 180): loop tests
  `position < size()` instead of `i` → reads past the end forever. Unused today.
  **Done:** now just calls `reset()`. Fixing the loop alone wouldn't work:
  `addByte()` never stores bytes past a completed packet, so the buffer after
  `position` only holds zeros, and re-adding them forms bogus `00 00` packets.
  The unit-test repo's full `Packetizer_Test` (incl. `RecycleNoInterleave`)
  plus the new bounds tests: 15/15 pass under ASan.
- [x] **3.4 `IbusPacket(std::array…)` constructors** (`IbusPacket.cpp:115-117,
  135-137`):
  - Length 253–255 reads past the 255-byte array. **[ASan-confirmed]**
    (heap-buffer-overflow).
  - CRC read from `raw[len+2]` instead of `raw[len+1]`, so every correct packet
    from `readIncomingQ_toLogic` reports invalid. **[confirmed]**
    (`valid=0 given=0 actual=0x72`). Nothing calls `isPacketValid()` yet.
  - `len > 255` is always false for `uint8_t`.
  **Done:** both array constructors share `initFromFixedBuffer()`, which accepts
  length bytes 2..253 (`MIN_LENGTH_FIELD`/`MAX_LENGTH_FIELD`) and reads the CRC
  from `raw[len + 1]`. Lower bound added too: a length byte of 1 gave a 3-byte
  packet, so `getDataLength()` (`size - 4` as `uint8_t`) returned 255 and the
  observers' `getDataLength() >= 2` guard let them read `getRawPacket()[3..4]`
  past the end. Null `unique_ptr` handled. 8 new host tests pass under ASan; on
  the old code 6 fail (overflow at len 253/254/255, valid packets reported
  invalid, null deref). `isPacketValid()` is now correct for packets from the
  queues (still unused). Both builds compile.
  Note: the unit-test repo's `IbusDataTest.cpp` calls `IbusPacket::getData()`,
  which no longer exists, so it doesn't build against current firmware (see 5).
- [x] **3.5 `writePacketToQ`** (`SingleCoreDmaManager.cpp:607-610`): copies the
  packet into a 255-byte stack array with a `uint8_t` index; packets >= 256
  bytes write `[255]` past the end, then wrap. Pico→Pi protobuf messages can be
  up to 255 + 4 bytes (logs are truncated to 64 chars; config messages are the
  risk).
  **Done:** packets larger than the 255-byte queue item are dropped (counted in
  `oversizedPacketsDropped`, shown by `writeStatus()`; no logging there because
  it can run inside `packetizerCs` and a log line can become a packet); the copy
  is a bounded `std::copy`. At the source, `BaseOutputWriter` now caps the
  protobuf payload at 251 bytes, so oversized Pico->Pi messages fail to encode
  instead (host check: largest that encodes is a 255-byte packet). Both builds
  compile. Can't host-test `writePacketToQ` itself (Pico queue API).
- [x] **3.6 `writeOutgoingQ_toUart`** (line 436): sends `buf[1] + 2` bytes from
  a 255-byte stack array → reads up to 2 bytes past the end for length 254/255.
  **Done:** only sends when the length byte is 2..253 (same range as 3.4);
  others are counted in `malformedOutgoingPacketsDropped` (shown by
  `writeStatus()`). The lower bound also stops an empty/unparseable packet
  (length byte 0) from putting a bare `00 00` on the car's bus. Both builds
  compile. Can't host-test (Pico UART/queue API).

## 4. Lower severity (logic, not memory)

- [x] **4.1** `BaseObserver::decodePiToPicoMessage` builds input via
  `std::make_unique<std::string>(inputString.c_str())`, which truncates the
  protobuf at the first `0x00`. Pass `inputString` itself.
  **Done**, plus two related bugs in the same path:
  - Truncation: payload is now built from the packet's byte range. In practice
    (proto3, protobuf-java on the Pi, no NUL in strings) a `0x00` byte appears
    only for an empty sub-message, e.g. a ConfigPush whose config is all
    defaults (`08 04 12 00`), which the old code lost.
  - **Uninitialized `messageType` on decode failure** (more serious): the
    function returned a default-initialized `PiToPicoMessage`, so observers
    acted on leftover memory. Host test: the same corrupt input came back as
    `ConfigPush` (with a full config) or `HeartbeatRequest` depending on what
    was on the stack. Now `messageType` defaults to a new `EmptyMessage`
    enumerator and failures return it explicitly; observers ignore it.
  - `MessageType` decode had no case for proto value 0 or unknown values and
    fell off the end of the function (UB). Same for `VideoSource` and
    `AspectRatio` in `ConfigMessage.h` (unknown values only). All now return a
    fallback.
  Also stopped calling `getRawPacket()` (a full copy) once per byte. Both
  builds compile; all observer switches on `messageType` have `default`.
- [x] **4.3** `proto_cpp/PicoToPi.h`: `MessageType` encode/decode also lack a
  fallback and `messageType` is uninitialized by default. Low risk: the Pico
  only encodes these, always with an explicit type.
  **Done:** same fix as 4.1: `EmptyMessage` enumerator (proto 0) as the default
  `messageType`, mapped both ways, and fallbacks in encode/decode. Hardening
  only: every construction site uses aggregate init with an explicit type.
  Host check: encoded wire bytes for all 7 message types are identical to the
  old code. No `.proto` change. Both builds compile.
- [x] **4.2** `IgnitionStatusRequestWriter.cpp:21`: `std::vector<uint8_t>(1)`
  then `push_back(0x10)` sends `{0x00, 0x10}`; probably want `{0x10}`.
  **Done:** payload is now `std::vector<uint8_t>{0x10}`.

## 5. Unit-test repo (`~/e39-rpi-fw-unittest`)

- [ ] Update the submodule (pinned at old `f638c6c`) to current firmware.
- [ ] Build with `-fsanitize=address,undefined -D_GLIBCXX_ASSERTIONS`.
- [ ] Tests: length bytes 0/253/254/255 on `Packetizer` and `IbusPacket`;
  config round-trip through a fake flash buffer; `drawScanline` with a line of
  alternating pixels.
