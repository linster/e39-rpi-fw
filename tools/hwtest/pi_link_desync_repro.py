"""Reproduce issue #16: one stray byte on the Pi link swallows the Pi-to-Pico messages after it.

Needs firmware built with -DE39_BENCH_TX_TEST=ON (SingleCoreDmaManager's benchInjectFromPi hook) and
an openocd server (see README). Everything goes over SWD: bytes are fed into the Pi UART's receive
path as if the Pi had sent them. Nothing is sent on the car bus (the dongle isn't used), because:
  * the test packets are heartbeat requests from the Pi to the Pico (81 04 31 08 01 bd), which the
    Pico doesn't forward to the car (#15);
  * the bogus frame the stray byte starts is never completed (it would need ~130 bytes; at most
    --packets x 6 are fed before the packetizer is reset). A completed bogus frame could pass the
    checksum by chance and be forwarded to the car.

Steps:
  1. clean injection (packetizer reset first): one heartbeat -> must be dispatched (sanity check)
  2. raw injection (as the real UART would deliver it): one stray byte (0x00, like a break)
  3. raw injection: --packets heartbeats -> with the bug, none are dispatched
  4. clean injection: one heartbeat, which also resets the packetizer (leaves the Pico clean)

Exit code: 1 if the bug reproduced, 0 if the heartbeats got through, 2 if the sanity check failed.

usage: pi_link_desync_repro.py --elf PicoTemplate.elf [--packets 5]
"""
import argparse, re, subprocess, sys, time
from swd_watch import Ocd

HEARTBEAT = bytes.fromhex('8104310801bd')   # Pi (0x81) -> Pico (0x31), PiToPico messageType = HeartbeatRequest
STRAY = bytes([0x00])

ap = argparse.ArgumentParser()
ap.add_argument('--elf', required=True)
ap.add_argument('--packets', type=int, default=5)
args = ap.parse_args()
if args.packets * len(HEARTBEAT) >= 120:
    raise SystemExit('--packets too high: could complete the bogus frame (see docstring); refusing')

ocd = Ocd()
nm = subprocess.run(['arm-none-eabi-nm', '-C', args.elf], capture_output=True, text=True).stdout


def sym(name):
    for line in nm.splitlines():
        if line.endswith('SingleCoreDmaManager::' + name):
            return int(line.split()[0], 16)
    raise SystemExit(f'{name} not in {args.elf}: build with -DE39_BENCH_TX_TEST=ON from a branch that has it')


BUF, LEN, RAW, DISPATCHED = (sym(n) for n in
                             ('benchInjectFromPi', 'benchInjectFromPiLen', 'benchInjectFromPiRaw',
                              'fromPiPacketsDispatched'))


def dispatched():
    return int(re.search(r':\s*([0-9a-fA-F]{8})', ocd.cmd(f'rp2040.core0 mdw 0x{DISPATCHED:08x}')).group(1), 16)


def inject(data, raw):
    ocd.cmd(f'rp2040.core0 mwb 0x{RAW:08x} {1 if raw else 0}')
    for i, b in enumerate(data):
        ocd.cmd(f'rp2040.core0 mwb 0x{BUF + i:08x} 0x{b:02x}')
    ocd.cmd(f'rp2040.core0 mwb 0x{LEN:08x} {len(data)}')   # core 0 consumes it and sets LEN back to 0
    deadline = time.time() + 2
    while time.time() < deadline:
        if int(re.search(r':\s*([0-9a-fA-F]{2})', ocd.cmd(f'rp2040.core0 mdb 0x{LEN:08x} 1')).group(1), 16) == 0:
            break
        time.sleep(0.02)
    else:
        raise SystemExit('core 0 never consumed the injected bytes; is the Pico running a bench build?')
    time.sleep(0.2)   # let the packet reach the observers


def step(label, data, raw, repeat=1):
    before = dispatched()
    for _ in range(repeat):
        inject(data, raw)
    got = dispatched() - before
    print(f'{label:58s} dispatched {got}/{repeat if data != STRAY else 0}', flush=True)
    return got


ok = step('1. clean: heartbeat (sanity check)', HEARTBEAT, raw=False)
if ok != 1:
    print('sanity check failed: a clean heartbeat was not dispatched; is the Pico running this build?')
    sys.exit(2)
step('2. raw:   one stray byte 00', STRAY, raw=True)
got = step(f'3. raw:   {args.packets} heartbeats after the stray byte', HEARTBEAT, raw=True, repeat=args.packets)
step('4. clean: heartbeat (resets the packetizer)', HEARTBEAT, raw=False)
print('cores:', ocd.cmd('rp2040.core0 curstate'), ocd.cmd('rp2040.core1 curstate'))

if got == 0:
    print(f'REPRODUCED (#16): none of the {args.packets} heartbeats after one stray byte reached the observers')
    sys.exit(1)
print(f'not reproduced: {got}/{args.packets} heartbeats got through after the stray byte')
sys.exit(0)
