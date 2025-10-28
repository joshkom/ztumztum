#!/usr/bin/env python3
import argparse
import lief
import sys

def patch_dt_init(input_path: str, output_path: str):
    # Load ELF
    elf = lief.parse(input_path)
    if elf is None:
        print(f"[!] Failed to parse ELF: {input_path}")
        sys.exit(1)

    # Get .init section
    init_section = elf.get_section(".init")
    if init_section is None:
        print("[!] No .init section found. Cannot add DT_INIT.")
        sys.exit(1)
    init_va = init_section.virtual_address
    print(f"[+] .init section VA: 0x{init_va:x}")

    # Find first DT_NULL entry to overwrite
    dt_null = next((entry for entry in elf.dynamic_entries if entry.tag == 0), None)
    if dt_null is None:
        print("[!] No DT_NULL entry found to patch DT_INIT. Cannot continue.")
        sys.exit(1)

    print(f"[+] Overwriting DT_NULL with DT_INIT pointing to 0x{init_va:x}")
    dt_null.tag = 12      # DT_INIT
    dt_null.value = init_va

    # Write patched ELF
    elf.write(output_path)
    print(f"[+] Patched ELF written to: {output_path}")


def main():
    parser = argparse.ArgumentParser(
        description="Patch ELF shared library to add DT_INIT pointing to .init section"
    )
    parser.add_argument("input", help="Input shared library (.so)")
    parser.add_argument("output", help="Output patched shared library (.so)")
    args = parser.parse_args()

    patch_dt_init(args.input, args.output)


if __name__ == "__main__":
    main()

