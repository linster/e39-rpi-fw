"""Non-halting soak watcher.  usage: soak_watch.py SECONDS ELF"""
import collections, re, sys, time
from swd_watch import Ocd, symbols, func_at, WATCHDOG_REASON

DHCSR = 0xE000EDF0
S_HALT, S_LOCKUP = 1 << 17, 1 << 19

def short(name): return re.sub(r'\(.*', '', name).split('::')[-1] if name else '?'

def main():
    seconds, elf = float(sys.argv[1]), sys.argv[2]
    sym, funcs = symbols(elf)
    ocd = Ocd()
    def word(c, a):
        m = re.search(r':\s*([0-9a-fA-F]{8})', ocd.cmd(f'rp2040.core{c} mdw 0x{a:08x}'))
        return int(m.group(1), 16) if m else None
    reason0 = word(0, WATCHDOG_REASON)
    start = time.time(); last_summary = start
    hist = {0: collections.Counter(), 1: collections.Counter()}
    while time.time() - start < seconds:
        t = time.time() - start
        for c in (0, 1):
            dh = word(c, DHCSR)
            pcs = [word(c, 0xE000101C) for _ in range(4)]
            names = [func_at(funcs, p & ~1) for p in pcs if p not in (None, 0xffffffff)]
            for n in names: hist[c][short(n)] += 1
            in_fault = any('isr_hardfault' in n or 'panic' in n for n in names)
            if (dh & S_LOCKUP) or (dh & S_HALT) or in_fault:
                why = 'LOCKUP' if dh & S_LOCKUP else ('halted (breakpoint)' if dh & S_HALT else 'in isr_hardfault/panic')
                print(f"[{t:7.1f}s] FAULT core{c}: {why}; DHCSR=0x{dh:08x}; samples: {', '.join(names) or '-'}", flush=True)
                return c
        reason = word(0, WATCHDOG_REASON)
        if reason != reason0:
            print(f"[{t:7.1f}s] WATCHDOG/RESET: reason 0x{reason0:x} -> 0x{reason:x}", flush=True); return 'reset'
        if time.time() - last_summary >= 60:
            for c in (0, 1):
                top = ', '.join(f'{n} {k}' for n, k in hist[c].most_common(4))
                print(f"[{t:7.1f}s] core{c} running; top: {top}", flush=True)
            hist = {0: collections.Counter(), 1: collections.Counter()}; last_summary = time.time()
        time.sleep(2)
    print(f"soak_watch: no fault in {seconds:.0f}s", flush=True)

if __name__ == '__main__':
    r = main()
    sys.exit(0 if r is None else 1)
