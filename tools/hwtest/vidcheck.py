"""Is scanvideo's pixel state machine actually outputting lines?"""
import collections
from swd_watch import Ocd
ocd = Ocd(); ocd.cmd('targets rp2040.core0')
def loop(addr, n=1500):
    out = ocd.cmd(f'set r {{}}; for {{set i 0}} {{$i < {n}}} {{incr i}} {{ mem2array a 32 0x{addr:08x} 1; lappend r $a(0) }}; return $r')
    return [int(x) for x in out.split() if x.isdigit()]
pc0 = collections.Counter(loop(0x502000d4))
gpio = loop(0xd0000004)
def pin(p): bits = [(v >> p) & 1 for v in gpio]; return sum(bits), sum(a != b for a, b in zip(bits, bits[1:]))
sw = gpio[-1]; src = ['RVC', 'UPSTREAM', 'PICO', 'PI'][((sw >> 19) & 1) * 2 + ((sw >> 20) & 1)]
print(f"  SM0 pc histogram: {dict(sorted(pc0.items()))}")
print(f"  colour pins (high, toggles): " + ' '.join(f"GP{p}={pin(p)}" for p in range(6)))
print(f"  sync pins (high, toggles): GP6={pin(6)} GP7={pin(7)}   video switch: {src}")
