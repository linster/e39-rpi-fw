"""Which events wedge scanvideo's scanline state machine?

Needs firmware built with -DE39_BENCH_TX_TEST=ON (BenchVideoTestObserver). Sends ONLY single
requests 14 -> 31 (14 is unused by E39 modules; 31 is the Pico), each after >= 1.5 ms of bus
silence and ~1.5 s apart:
  7C <ms>       core 1 busy-waits <ms> (no scanlines delivered), no scan program switch
  7D <program>  switch scan programs (0 BOOT_SPLASH, 1 CLOCK, 3 MENU)
After each request it reads ScanProgramManager::scanlineSmRecoveries (the watchdog's restart count).

usage: wedge_test.py --elf PicoTemplate.elf [--stalls 2,5,10,20,50,100,200] [--repeats 3] [--swaps 6]
"""
import argparse, os, re, select, subprocess, termios, time
from serialport import open_8e1
from swd_watch import Ocd

REQ_SRC, PICO = 0x14, 0x31
ap = argparse.ArgumentParser()
ap.add_argument('--elf', required=True)
ap.add_argument('--stalls', default='2,5,10,20,50,100,200')
ap.add_argument('--repeats', type=int, default=3)
ap.add_argument('--swaps', type=int, default=6)
ap.add_argument('--bootsplash', type=int, default=2)
ap.add_argument('--dev', default='/dev/ttyUSB0')
args = ap.parse_args()

fd = open_8e1(args.dev)
ocd = Ocd()
nm = subprocess.run(['arm-none-eabi-nm', '-C', args.elf], capture_output=True, text=True).stdout
RECOVERIES = next(int(l.split()[0], 16) for l in nm.splitlines() if l.endswith('ScanProgramManager::scanlineSmRecoveries'))


def recoveries():
    return int(re.search(r':\s*([0-9a-fA-F]{8})', ocd.cmd(f'rp2040.core0 mdw 0x{RECOVERIES:08x}')).group(1), 16)


def wait_for_idle_bus(timeout_s=0.25):
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        r, _, _ = select.select([fd], [], [], 0.0015)
        if not r:
            return
        try:
            os.read(fd, 4096)
        except BlockingIOError:
            pass


def request(command, arg):
    p = bytes([REQ_SRC, 4, PICO, command, arg])
    c = 0
    for b in p:
        c ^= b
    wait_for_idle_bus()
    os.set_blocking(fd, True)
    os.write(fd, p + bytes([c]))
    termios.tcdrain(fd)
    os.set_blocking(fd, False)


def trial(label, command, arg):
    before = recoveries()
    request(command, arg)
    time.sleep(1.5)
    after = recoveries()
    return after - before


results = {}
for ms in [int(x) for x in args.stalls.split(',')]:
    wedges = [trial(f'stall {ms} ms', 0x7C, ms) for _ in range(args.repeats)]
    results[f'core 1 stall {ms:3d} ms'] = wedges
    print(f'core 1 stall {ms:3d} ms: recoveries per request {wedges}', flush=True)
swaps = []
for i in range(args.swaps * 2):
    swaps.append(trial('swap', 0x7D, 1 if i % 2 == 0 else 3))
print(f'switch clock<->menu: recoveries per switch {swaps}', flush=True)
boots = []
for _ in range(args.bootsplash):
    boots.append(trial('bootsplash', 0x7D, 0))
    request(0x7D, 1); time.sleep(1.5)   # back to the clock, so the next bootsplash switch isn't a no-op
print(f'switch to bootsplash (then back to clock): recoveries {boots}', flush=True)
