"""Stack high-water marks via SWD.  usage: stack_paint.py paint | stack_paint.py read"""
import re, sys, time
from swd_watch import Ocd
PAT = 0xA5A5A5A5
import os, subprocess
ELF = os.environ.get('ELF')
REGIONS = {0: (0x20041000, 0x20042000), 1: (0x20040000, 0x20041000)}   # default: SCRATCH_Y = core0, SCRATCH_X = core1
if ELF:
    for line in subprocess.run(['arm-none-eabi-nm', '-C', '-S', ELF], capture_output=True, text=True).stdout.splitlines():
        f = line.split()
        if len(f) == 4 and f[3] == 'core1Stack':
            a, n = int(f[0], 16), int(f[1], 16)
            REGIONS = {0: (0x20040000, 0x20042000), 1: (a, a + n)}  # core1 has its own stack; core0 owns both scratch banks
ocd = Ocd()
def sp_of(c):
    ocd.cmd(f'targets rp2040.core{c}')
    return int(re.search(r'0x[0-9a-f]+', ocd.cmd('reg msp force')).group(0), 16)
def lowest_used(lo, hi):
    a = lo
    while a < hi:
        raw = ocd.cmd(f'rp2040.core0 mdw 0x{a:08x} 64')
        for line in raw.splitlines():
            m = re.match(r'0x([0-9a-f]+):\s*(.*)', line.strip())
            if m:
                base = int(m.group(1), 16)
                for i, w in enumerate(m.group(2).split()):
                    if int(w, 16) != PAT: return base + 4 * i
        a += 256
    return hi
if sys.argv[1] == 'paint':
    for c in (0, 1):
        ocd.cmd(f'targets rp2040.core{c}'); ocd.cmd('halt')
    for c in (0, 1):
        lo, hi = REGIONS[c]; sp = sp_of(c)
        end = min(sp - 64, hi)
        # core0 may legitimately be using anything above its sp; paint only below it.
        ocd.cmd(f'rp2040.core0 mwb 0x{lo:08x} 0xA5 {end - lo}')
        print(f"core{c}: sp=0x{sp:08x}, painted 0x{lo:08x}-0x{end:08x} ({end - lo} bytes)")
    for c in (1, 0):
        ocd.cmd(f'targets rp2040.core{c}'); ocd.cmd('resume')
else:
    for c in (0, 1):
        lo, hi = REGIONS[c]; low = lowest_used(lo, hi)
        print(f"core{c}: region 0x{lo:08x}-0x{hi:08x}; lowest overwritten word 0x{low:08x} -> peak stack use {hi - low} of {hi - lo} bytes"
              + ("  ** REACHED THE BOTTOM: overflowed into the next region **" if low == lo else ''))
