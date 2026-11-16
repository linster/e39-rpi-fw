"""Print the Pico's stored config: the E39C frame in the persistent flash sector (read via XIP, no halt)."""
import re, hashlib
from swd_watch import Ocd
ocd = Ocd()
out = ocd.cmd('rp2040.core0 mdb 0x101ff000 256')
b = []
for line in out.splitlines():
    m = re.match(r'0x[0-9a-f]+:\s*(.*)', line.strip())
    if m: b += [int(x, 16) for x in m.group(1).split()]
if bytes(b[:4]) == b'E39C':
    n = b[4] | (b[5] << 8); payload = bytes(b[6:6 + n])
    print(f"  flash: E39C frame, {n}-byte payload {payload.hex(' ')}  (sha1 {hashlib.sha1(bytes(b)).hexdigest()[:10]})")
else:
    print(f"  flash: no frame: {' '.join(f'{x:02x}' for x in b[:12])}")
