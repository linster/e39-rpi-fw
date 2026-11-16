"""Drive the Pico's menu over IBus.

WARNING: this sends REAL BMBT and IKE messages that other modules on the bus also receive:
  * telephone long press  F0 -> FF  48 48   (broadcast; opens the Pico menu)
  * knob turn             F0 -> 3B  49 xx
  * knob press            F0 -> 3B  48 05   (activates the focused menu item, e.g. a Pi power off)
  * ignition position 1   80 -> BF  11 01   (broadcast; the Pico switches to the clock and powers the Pi)
Only run it on a bench where that's acceptable, and pass --sends-real-bmbt-and-ike-messages.

usage: menu_test.py --sends-real-bmbt-and-ike-messages visual|stress [SECONDS] [--press]
  visual: long press, knob right x3, left x1, 3 s apart (add --press to press the knob at the end)
  stress: alternate menu / clock every 0.3-1.5 s with bursts of knob turns, for SECONDS
"""
import argparse, os, random, select, termios, time
from serialport import open_8e1

BUS_IDLE_S = 0.0015
GAP_S = 0.010

ap = argparse.ArgumentParser()
ap.add_argument('--sends-real-bmbt-and-ike-messages', action='store_true', dest='ack')
ap.add_argument('mode', choices=['visual', 'stress'])
ap.add_argument('seconds', nargs='?', type=float, default=60)
ap.add_argument('--press', action='store_true')
ap.add_argument('--dev', default='/dev/ttyUSB0')
args = ap.parse_args()
if not args.ack:
    raise SystemExit('refusing: this sends real BMBT/IKE messages; pass --sends-real-bmbt-and-ike-messages')

fd = open_8e1(args.dev)


def wait_for_idle_bus(timeout_s=0.25):
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        r, _, _ = select.select([fd], [], [], BUS_IDLE_S)
        if not r:
            return
        try:
            os.read(fd, 4096)
        except BlockingIOError:
            pass


def send(src, dst, data):
    p = bytes([src, len(data) + 2, dst]) + bytes(data)
    c = 0
    for b in p:
        c ^= b
    wait_for_idle_bus()
    os.set_blocking(fd, True)
    os.write(fd, p + bytes([c]))
    termios.tcdrain(fd)
    os.set_blocking(fd, False)
    time.sleep(GAP_S)


long_press = lambda: send(0xF0, 0xFF, [0x48, 0x48])
knob_right = lambda n=1: send(0xF0, 0x3B, [0x49, 0x80 + n])
knob_left = lambda n=1: send(0xF0, 0x3B, [0x49, n])
knob_press = lambda: send(0xF0, 0x3B, [0x48, 0x05])
ignition_1 = lambda: send(0x80, 0xBF, [0x11, 0x01])

if args.mode == 'visual':
    steps = [('long press -> menu', long_press), ('knob right', knob_right), ('knob right', knob_right),
             ('knob right', knob_right), ('knob left', knob_left)]
    if args.press:
        steps.append(('knob press', knob_press))
    t0 = time.time()
    for name, f in steps:
        print(f'[{time.time() - t0:4.1f}s] {name}', flush=True)
        f()
        time.sleep(3)
else:
    rng = random.Random(7)
    end = time.time() + args.seconds
    n = {'menu': 0, 'clock': 0, 'knob': 0}
    while time.time() < end:
        if rng.random() < 0.5:
            long_press(); n['menu'] += 1
        else:
            ignition_1(); n['clock'] += 1
        for _ in range(rng.randrange(5, 25)):
            (knob_right if rng.random() < 0.5 else knob_left)(rng.randrange(1, 4)); n['knob'] += 1
            time.sleep(rng.uniform(0.0, 0.05))
        time.sleep(rng.uniform(0.3, 1.5))
    print(f"stress done: {n['menu']} menu switches, {n['clock']} clock switches, {n['knob']} knob turns")
