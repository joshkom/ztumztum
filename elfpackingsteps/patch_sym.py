#!/usr/bin/env python3
import lief
import sys

if len(sys.argv) != 4:
    print("Usage: python rename_dynsym.py <input.so> <old_symbol> <new_symbol>")
    sys.exit(1)

infile = sys.argv[1]
old_name = sys.argv[2]
new_name = sys.argv[3]
outfile = infile.replace(".so", "_renamed.so")

# Parse the ELF binary
binary = lief.parse(infile)
if not binary:
    print("Failed to parse ELF:", infile)
    sys.exit(1)

found = False

# Rename dynamic symbols (undefined or exported)
for sym in binary.dynamic_symbols:
    if sym.name == old_name:
        print(f"[+] Renaming dynamic symbol: {old_name} -> {new_name}")
        sym.name = new_name
        found = True

if not found:
    print(f"[!] Symbol '{old_name}' not found in dynamic symbols")
    sys.exit(1)

# Write the patched ELF
binary.write(outfile)
print(f"[+] Patched library written to {outfile}")

# Verify
print("[+] Verify:")
print("    readelf -s", outfile, "| grep", new_name)
print("    nm -D", outfile, "| grep", new_name)

