#!/usr/bin/env python3
import lief
import sys

if len(sys.argv) != 4:
    print("Usage: python3 set_entry.py <in.so> <new_entry_hex> <out.so>")
    sys.exit(1)

infile = sys.argv[1]
new_entry = int(sys.argv[2], 16) if sys.argv[2].startswith("0x") else int(sys.argv[2])
outfile = sys.argv[3]

bin = lief.parse(infile)
if not bin:
    print("Failed to parse:", infile)
    sys.exit(1)

old_entry = bin.header.entrypoint
print(f"[+] Old entrypoint: 0x{old_entry:x}")

# set new entrypoint (e_entry)
bin.header.entrypoint = new_entry

# Write patched file
bin.write(outfile)
print(f"[+] New entrypoint set: 0x{new_entry:x} -> {outfile}")
print("[+] Verify with: readelf -h", outfile)

