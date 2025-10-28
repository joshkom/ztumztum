#!/usr/bin/env python3
import subprocess
import hashlib
import sys
from pathlib import Path
import re

def parse_readelf_for_exec_end(filename: str) -> int:
    """
    Runs `readelf -lW` and finds the end offset of the executable LOAD segment.
    Returns the file offset where hashing should stop.
    """
    try:
        out = subprocess.check_output(["readelf", "-lW", filename], text=True)
    except subprocess.CalledProcessError as e:
        raise RuntimeError(f"Failed to run readelf: {e}")

    exec_end = 0
    load_segments = []

    # Regex to capture: LOAD offset ... FileSiz ... flags
    load_line_re = re.compile(
        r"LOAD\s+0x([0-9a-fA-F]+)\s+0x[0-9a-fA-F]+\s+0x[0-9a-fA-F]+\s+0x([0-9a-fA-F]+)\s+0x[0-9a-fA-F]+\s+([RWE\s]+)\s+0x[0-9a-fA-F]+"
    )

    for m in load_line_re.finditer(out):
        offset = int(m.group(1), 16)
        filesize = int(m.group(2), 16)
        flags = m.group(3).replace(" ", "").upper()  # normalize

        load_segments.append((offset, filesize, flags))

        # Treat any segment with E or X as executable
        if "E" in flags or "X" in flags:
            exec_end = max(exec_end, offset + filesize)

    # Logging all LOAD segments
    print("[+] Detected LOAD segments:")
    for i, (offset, size, flags) in enumerate(load_segments):
        print(f"    Segment {i}: offset=0x{offset:X}, size=0x{size:X}, flags={flags}")

    if exec_end == 0:
        raise RuntimeError("No executable LOAD segment found! Check readelf output.")

    return exec_end


def hash_upto_exec_end(filename: str):
    exec_end_offset = parse_readelf_for_exec_end(filename)
    print(f"[+] Hashing from file start (0x0) up to 0x{exec_end_offset:X} (end of exec segment)")

    with open(filename, "rb") as f:
        data = f.read(exec_end_offset)

    md5_hash = hashlib.md5(data).hexdigest()
    sha256_hash = hashlib.sha256(data).hexdigest()

    print(f"[+] MD5:    {md5_hash}")
    print(f"[+] SHA256: {sha256_hash}")

    return md5_hash, sha256_hash


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {Path(sys.argv[0]).name} <ELF-file>")
        sys.exit(1)

    hash_upto_exec_end(sys.argv[1])

