import sys
import mmap
import struct
import hashlib
import os

# === Colors ===
RESET = "\033[0m"
BOLD = "\033[1m"
CYAN = "\033[36m"
GREEN = "\033[32m"
YELLOW = "\033[33m"
RED = "\033[31m"
MAGENTA = "\033[35m"

# === Constants ===
MAGIC = b'\x41\x50\x4B\x20\x53\x69\x67\x20\x42\x6C\x6F\x63\x6B\x20\x34\x32'  # "APK Sig Block 42"

def read_le64(data):
    return struct.unpack('<Q', data)[0]

def main():
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <apk_file>")
        sys.exit(1)

    apk_path = sys.argv[1]

    file_size = os.path.getsize(apk_path)
    print(f"{YELLOW}File size:{RESET} {file_size:,} bytes")

    with open(apk_path, 'rb') as f:
        mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)

        magic_offset = mm.find(MAGIC)
        if magic_offset == -1:
            print(f"{RED}Magic string not found in file.{RESET}")
            mm.close()
            return

        print(f"{CYAN}Magic string offset:{RESET} {BOLD}{magic_offset}{RESET}")

        footer_size_offset = magic_offset - 8
        print(f"{YELLOW}Footer size offset:{RESET} {footer_size_offset}")

        footer_size = read_le64(mm[footer_size_offset:footer_size_offset + 8])
        print(f"{YELLOW}Footer size (blockSize):{RESET} {footer_size}")

        header_size_offset = magic_offset - footer_size + 8
        print(f"{YELLOW}Header size offset:{RESET} {header_size_offset}")

        header_size = read_le64(mm[header_size_offset:header_size_offset + 8])
        print(f"{YELLOW}Header size:{RESET} {header_size}")

        if footer_size != header_size:
            print(f"{RED}Header and footer size mismatch!{RESET}")
            mm.close()
            return

        block_start = header_size_offset
        block_end = header_size_offset + footer_size + 8
        print(f"{YELLOW}Reading APK Signing Block:{RESET} {block_start} → {block_end}")

        # Compute SHA-256 directly from mmap
        hasher = hashlib.sha256()
        hasher.update(mm[block_start:block_end])
        sha256_hash = hasher.hexdigest()

        print(f"{GREEN}SHA-256:{RESET} {BOLD}{sha256_hash}{RESET}")

        mm.close()

if __name__ == "__main__":
    main()

