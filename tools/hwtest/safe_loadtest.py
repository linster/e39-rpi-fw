"""Safe IBus load test for the Pico on a bus shared with real BMW modules.

Only two kinds of packet are ever sent:
  * load:  14 -> 12 with random data. No E39 module uses these addresses, so nothing acts on them,
           but the Pico has to receive and packetize every byte.
  * probe: 81 -> 31 (Pi -> Pico) ConfigPush with a unique hash; only the Pico acts on it. The probe
           checks the Pico is still handling input, by reading the saved hash back from flash.

Bus etiquette (see README.md): before each packet, wait for >= 1.5 ms of bus silence (I/K-bus devices
only start transmitting after ~1.5 ms idle), and leave --gap-ms between our packets (default 20 ms) so
real modules always get the bus. Flooding back to back starves them: on the bench that made the IKE's
signal lights flicker even with unused addresses.

usage: safe_loadtest.py --elf PicoTemplate.elf [--seconds 600] [--gap-ms 20] [--probe-every 60] [--dev /dev/ttyUSB0]
Needs an openocd server with tcl_port 6666 (see README.md).
"""
import argparse, os, random, re, select, subprocess, termios, time
from serialport import open_8e1
from swd_watch import Ocd

LOAD_SRC, LOAD_DST = 0x14, 0x12
PI, PICO = 0x81, 0x31
ALLOWED = {(LOAD_SRC, LOAD_DST), (PI, PICO)}
COUNTERS = ['rxBytesDropped', 'oversizedPacketsDropped', 'malformedOutgoingPacketsDropped']
BUS_IDLE_S = 0.0015


def counter_addresses(elf):
    out = subprocess.run(['arm-none-eabi-nm', '-C', elf], capture_output=True, text=True).stdout
    addrs = {}
    for line in out.splitlines():
        for name in COUNTERS:
            if line.endswith('SingleCoreDmaManager::' + name):
                addrs[name] = int(line.split()[0], 16)
    return addrs


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--elf', required=True)
    ap.add_argument('--seconds', type=float, default=600)
    ap.add_argument('--gap-ms', type=float, default=20)
    ap.add_argument('--probe-every', type=float, default=60)
    ap.add_argument('--dev', default='/dev/ttyUSB0')
    args = ap.parse_args()
    if args.gap_ms < 10:
        raise SystemExit('--gap-ms below 10 ms risks starving real modules; refusing')

    fd = open_8e1(args.dev)
    ocd = Ocd()
    rng = random.Random(1)
    addrs = counter_addresses(args.elf)

    def word(a):
        return int(re.search(r':\s*([0-9a-fA-F]{8})', ocd.cmd(f'rp2040.core0 mdw 0x{a:08x}')).group(1), 16)

    def counters():
        return {k: word(a) for k, a in addrs.items()}

    def wait_for_idle_bus(timeout_s=0.25):
        """Wait until nothing has been received for BUS_IDLE_S (or give up after timeout_s)."""
        deadline = time.time() + timeout_s
        while time.time() < deadline:
            r, _, _ = select.select([fd], [], [], BUS_IDLE_S)
            if not r:
                return True
            try:
                os.read(fd, 4096)
            except BlockingIOError:
                pass
        return False

    def send(src, dst, data):
        if (src, dst) not in ALLOWED:
            raise SystemExit(f'refusing to send {src:02x} -> {dst:02x}')
        p = bytes([src, len(data) + 2, dst]) + bytes(data)
        c = 0
        for b in p:
            c ^= b
        p += bytes([c])
        wait_for_idle_bus()
        os.set_blocking(fd, True)
        os.write(fd, p)
        termios.tcdrain(fd)
        os.set_blocking(fd, False)
        time.sleep(args.gap_ms / 1000)
        return len(p)

    def config_push(h):
        h = h.encode()
        cfg = bytes([0x0A, len(h)]) + h + bytes([0x18, 0x05, 0x20, 0x01, 0x28, 0x01, 0x30, 0x01, 0x38, 0x01, 0x58, 0x01])
        send(PI, PICO, list(bytes([0x08, 0x04, 0x12, len(cfg)]) + cfg))

    def stored_hash():
        out = ocd.cmd('rp2040.core0 mdb 0x101ff000 40')
        b = []
        for line in out.splitlines():
            m = re.match(r'0x[0-9a-f]+:\s*(.*)', line.strip())
            if m:
                b += [int(x, 16) for x in m.group(1).split()]
        if bytes(b[:4]) != b'E39C' or b[6] != 0x0A:
            return None
        return bytes(b[8:8 + b[7]]).decode(errors='replace')

    start = time.time()
    next_probe = start + 10
    sent = probes = probes_ok = 0
    c0 = counters()
    print(f'start: {c0}  (gap {args.gap_ms} ms, idle wait {BUS_IDLE_S * 1000:.1f} ms)', flush=True)
    while time.time() - start < args.seconds:
        sent += send(LOAD_SRC, LOAD_DST, [rng.randrange(256) for _ in range(rng.randrange(1, 12))])
        if time.time() >= next_probe:
            probes += 1
            t = time.time() - start
            h = f'load-{probes}'
            config_push(h)
            time.sleep(3)
            got = stored_hash()
            probes_ok += got == h
            c = counters()
            occupancy = sent * 11 / 9600 / max(t, 1) * 100   # 11 bits per byte at 8E1
            print(f'[{t:6.1f}s] sent {sent} B (~{occupancy:.0f}% bus)  probe {h!r} -> {got!r} '
                  f'{"OK" if got == h else "MISSED"}  ' + ' '.join(f'{k} {v}' for k, v in c.items()), flush=True)
            next_probe = time.time() + args.probe_every
    c1 = counters()
    print(f'done: {sent} B in {args.seconds:.0f} s, probes {probes_ok}/{probes} OK; deltas: '
          + ', '.join(f'{k} +{c1[k] - c0[k]}' for k in c0), flush=True)


if __name__ == '__main__':
    main()
