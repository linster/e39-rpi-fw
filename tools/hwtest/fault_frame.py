"""Dump the exception frame of a core sitting in isr_hardfault.  usage: fault_frame.py ELF CORE"""
import re, subprocess, sys
from swd_watch import Ocd, symbols, func_at
elf, core = sys.argv[1], int(sys.argv[2])
sym, funcs = symbols(elf)
ocd = Ocd()
ocd.cmd(f'targets rp2040.core{core}'); ocd.cmd('halt')
def reg(n): return int(re.search(r'0x[0-9a-f]+', ocd.cmd(f'reg {n} force')).group(0), 16)
lr, msp, psp = reg('lr'), reg('msp'), reg('psp')
sp = psp if (lr & 0x4) else msp
frame = [int(x, 16) for x in re.findall(r'\b[0-9a-f]{8}\b', ocd.cmd(f'mdw 0x{sp:08x} 8').split(':', 1)[1])]
names = ['r0', 'r1', 'r2', 'r3', 'r12', 'lr', 'pc', 'xpsr']
print(f"core{core}: EXC_RETURN lr=0x{lr:08x} ({'PSP' if lr & 4 else 'MSP'}), frame at 0x{sp:08x}")
for n, v in zip(names, frame):
    where = f"  <- {func_at(funcs, v & ~1)}" if n in ('pc', 'lr') else ''
    print(f"  {n:5s} 0x{v:08x}{where}")
print(f"  faulting exception number in stacked xPSR: {frame[7] & 0x3f}")
# a few return addresses further up the stack, for context
stack = [int(x, 16) for x in re.findall(r'\b[0-9a-f]{8}\b', ocd.cmd(f'mdw 0x{sp + 32:08x} 48').split(':', 1)[1])] if True else []
text = [v for v in stack if 0x10000000 <= (v & ~1) < 0x10200000 and (v & 1)]
print("  code addresses further up the stack:")
for v in text[:10]: print(f"    0x{v:08x}  {func_at(funcs, v & ~1)}")
ocd.cmd('resume')
