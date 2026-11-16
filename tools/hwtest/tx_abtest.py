"""Car bus transmitter test: how many of the Pico's packets arrive intact while the bus is busy?

Needs firmware built with -DE39_BENCH_TX_TEST=ON (BenchTxTestObserver). Sends ONLY:
  * request:    14 -> 31  7E <n>   (asks the Pico to transmit n test packets 31 -> 12)
  * contention: 14 -> 12  random 1-11 bytes, after >= 1.5 ms of bus silence, >= --gap-ms apart
14 and 12 aren't used by E39 modules; 31 is the Pico.

usage: tx_abtest.py --elf PicoTemplate.elf [--trials 3] [--packets 30] [--gap-ms 20] [--no-contention]
"""
import argparse, os, random, re, select, subprocess, termios, time
from serialport import open_8e1
from swd_watch import Ocd

REQ_SRC, PICO, TEST_DST, MAGIC = 0x14, 0x31, 0x12, 0x7E
BUS_IDLE_S = 0.0015
COUNTERS = ['carTxSent', 'carTxBusyDeferrals', 'carTxCollisions', 'carTxDroppedAfterRetries', 'outgoingQueueFullDrops']

ap = argparse.ArgumentParser()
ap.add_argument('--elf', required=True)
ap.add_argument('--trials', type=int, default=3)
ap.add_argument('--packets', type=int, default=30)
ap.add_argument('--gap-ms', type=float, default=20)
ap.add_argument('--trial-seconds', type=float, default=5)
ap.add_argument('--no-contention', action='store_true')
ap.add_argument('--dev', default='/dev/ttyUSB0')
args = ap.parse_args()
if args.gap_ms < 10:
    raise SystemExit('--gap-ms below 10 ms risks starving real modules; refusing')

fd = open_8e1(args.dev)
rx = bytearray()
rng = random.Random(3)


def read_for(seconds):
    end = time.time() + seconds
    while True:
        left = end - time.time()
        if left <= 0:
            return
        r, _, _ = select.select([fd], [], [], min(left, 0.05))
        if r:
            try:
                rx.extend(os.read(fd, 4096))
            except BlockingIOError:
                pass


def wait_for_idle_bus(timeout_s=0.25):
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        r, _, _ = select.select([fd], [], [], BUS_IDLE_S)
        if not r:
            return
        try:
            rx.extend(os.read(fd, 4096))
        except BlockingIOError:
            pass


def send(src, dst, data):
    assert (src, dst) in ((REQ_SRC, PICO), (REQ_SRC, TEST_DST)), 'refusing to send to any other address'
    p = bytes([src, len(data) + 2, dst]) + bytes(data)
    c = 0
    for b in p:
        c ^= b
    wait_for_idle_bus()
    os.set_blocking(fd, True)
    os.write(fd, p + bytes([c]))
    termios.tcdrain(fd)
    os.set_blocking(fd, False)
    read_for(args.gap_ms / 1000)


def packets(buf):
    out, i = [], 0
    while len(buf) - i >= 4:
        n = buf[i + 1]
        if n >= 2 and i + n + 2 <= len(buf):
            p = bytes(buf[i:i + n + 2])
            c = 0
            for b in p[:-1]:
                c ^= b
            if c == p[-1]:
                out.append(p)
                i += n + 2
                continue
        i += 1
    return out


ocd = Ocd()
nm = subprocess.run(['arm-none-eabi-nm', '-C', args.elf], capture_output=True, text=True).stdout
addr = {name: int(line.split()[0], 16) for line in nm.splitlines() for name in COUNTERS
        if line.endswith('SingleCoreDmaManager::' + name)}


def counters():
    return {k: int(re.search(r':\s*([0-9a-fA-F]{8})', ocd.cmd(f'rp2040.core0 mdw 0x{a:08x}')).group(1), 16)
            for k, a in addr.items()}


total = 0
for trial in range(1, args.trials + 1):
    rx.clear()
    c0 = counters()
    send(REQ_SRC, PICO, [MAGIC, args.packets])
    end = time.time() + args.trial_seconds
    while time.time() < end:
        if args.no_contention:
            read_for(0.05)
        else:
            send(REQ_SRC, TEST_DST, [rng.randrange(256) for _ in range(rng.randrange(1, 12))])
    read_for(0.5)
    pk = packets(rx)
    ours = {p[5] for p in pk if p[0] == PICO and p[2] == TEST_DST and len(p) >= 7 and p[3] == MAGIC and p[4] == trial}
    c1 = counters()
    total += len(ours)
    delta = ', '.join(f'{k} +{c1[k] - c0[k]}' for k in c0) if c0 else 'no TX counters in this build'
    print(f'trial {trial}: {len(ours)}/{args.packets} Pico packets intact on the bus  ({len(rx)} bytes seen, '
          f'{len(pk)} valid packets)  [{delta}]', flush=True)
print(f'total: {total}/{args.trials * args.packets} intact ({100 * total / (args.trials * args.packets):.0f}%)')
