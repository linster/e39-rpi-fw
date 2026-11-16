"""Poll both RP2040 cores through openocd's TCL port.  usage: swd_watch.py SECONDS ELF"""
import re, socket, subprocess, sys, time

WATCHDOG_REASON = 0x40058008      # bit0 TIMER, bit1 FORCE (RP2040 datasheet 4.7)
EXC = {0: 'thread', 2: 'NMI', 3: 'HARDFAULT', 11: 'SVCall', 14: 'PendSV', 15: 'SysTick'}

class Ocd:
    def __init__(self, port=6666):
        self.s = socket.create_connection(('127.0.0.1', port), timeout=10)
    def cmd(self, c):
        self.s.sendall(c.encode() + b'\x1a'); out = b''
        while not out.endswith(b'\x1a'): out += self.s.recv(4096)
        return out[:-1].decode(errors='replace').strip()
    def word(self, addr):
        m = re.search(r':\s*([0-9a-fA-F]{8})', self.cmd(f'mdw 0x{addr:08x}'))
        return int(m.group(1), 16) if m else None

def symbols(elf):
    out = subprocess.run(['arm-none-eabi-nm', '-C', elf], capture_output=True, text=True).stdout
    sym = {}
    for line in out.splitlines():
        parts = line.split(' ', 2)
        if len(parts) == 3: sym[parts[2]] = int(parts[0], 16)
    funcs = sorted((a, n) for n, a in sym.items())
    return sym, funcs

def func_at(funcs, pc):
    lo, hi = 0, len(funcs) - 1; best = None
    while lo <= hi:
        mid = (lo + hi) // 2
        if funcs[mid][0] <= pc: best = funcs[mid]; lo = mid + 1
        else: hi = mid - 1
    return f"{best[1]}+0x{pc - best[0]:x}" if best else '?'

def core_state(ocd, core):
    ocd.cmd(f'targets rp2040.core{core}'); ocd.cmd('halt')
    pc = int(re.search(r'0x[0-9a-f]+', ocd.cmd('reg pc force')).group(0), 16)
    xpsr = int(re.search(r'0x[0-9a-f]+', ocd.cmd('reg xPSR force')).group(0), 16)
    ocd.cmd('resume')
    return pc, xpsr & 0x3f

def main():
    seconds = float(sys.argv[1]); elf = sys.argv[2]
    sym, funcs = symbols(elf)
    dropped = sym.get('pico::ibus::dma::SingleCoreDmaManager::rxBytesDropped')
    ocd = Ocd()
    start = time.time(); end = start + seconds
    first_reason = ocd.word(WATCHDOG_REASON); last_print = 0; same_pc = {0: [], 1: []}
    while time.time() < end:
        states = [core_state(ocd, c) for c in (0, 1)]
        reason = ocd.word(WATCHDOG_REASON)
        drop = ocd.word(dropped) if dropped else None
        t = time.time() - start
        faulted = [c for c, (pc, exc) in enumerate(states) if exc == 3 or 'hardfault' in func_at(funcs, pc).lower() or 'panic' in func_at(funcs, pc)]
        for c, (pc, _) in enumerate(states):
            same_pc[c] = (same_pc[c] + [pc])[-8:]
        if faulted or reason != first_reason or t - last_print >= 10:
            desc = '; '.join(f"core{c}: {EXC.get(exc, 'exc%d' % exc)} @ {func_at(funcs, pc)}" for c, (pc, exc) in enumerate(states))
            extra = f", rxBytesDropped={drop}" if drop is not None else ''
            print(f"[{t:6.1f}s] {desc}{extra}, watchdog reason=0x{reason:x}", flush=True); last_print = t
        if faulted:
            print(f"FAULT on core(s) {faulted} at {t:.1f}s", flush=True); return
        if reason != first_reason:
            print(f"WATCHDOG/RESET: reason changed 0x{first_reason:x} -> 0x{reason:x} at {t:.1f}s", flush=True); return
        time.sleep(2)
    print(f"swd_watch: no fault in {seconds:.0f}s", flush=True)

if __name__ == '__main__':
    main()
