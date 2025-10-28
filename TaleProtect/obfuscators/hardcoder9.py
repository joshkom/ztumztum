#!/usr/bin/env python3
"""
hardcoder8.py

Generator that hardcodes data into instruction immediates and emits either:
 - a function that returns a malloc'ed buffer with decoded bytes, or
 - a macro (GCC/Clang compound-expression) that does the same inline and returns the buffer.

Usage:
    python3 hardcoder8.py --string Hello -o out.c --mode func
    python3 hardcoder8.py --string Hello -o out.h --mode macro

Notes:
 - Macro mode uses GCC/Clang compound-expression extension: ({ ... })
 - Caller must free() the returned buffer.
"""
from __future__ import annotations
import argparse, hashlib, random, textwrap, re

MASK64 = (1 << 64) - 1
BASE_OPS = ['xor', 'add', 'sub', 'rol', 'ror', 'mul']

def seeded_rng(seed_text: str):
    h = hashlib.sha256(seed_text.encode('utf-8')).digest()
    return random.Random(int.from_bytes(h[:8], 'little'))

def rol64(v, n):
    n &= 63
    return ((v << n) | (v >> (64 - n))) & MASK64

def ror64(v, n):
    n &= 63
    return ((v >> n) | (v << (64 - n))) & MASK64

def apply_seq_forward(val, seq, tmpA_final=None):
    v = val & MASK64
    for op, imm in seq:
        if op == 'xor': v ^= imm & MASK64
        elif op == 'add': v = (v + (imm & MASK64)) & MASK64
        elif op == 'sub': v = (v - (imm & MASK64)) & MASK64
        elif op == 'rol': v = rol64(v, imm & 63)
        elif op == 'ror': v = ror64(v, imm & 63)
        elif op == 'mul': v = (v * (imm & MASK64)) & MASK64
        elif op == 'xortmp':
            if tmpA_final is None: raise RuntimeError("xortmp requires tmpA_final")
            v ^= tmpA_final
    return v

def invert_seq(target, seq, tmpA_final=None):
    v = target & MASK64
    for op, imm in reversed(seq):
        if op == 'xor': v ^= imm & MASK64
        elif op == 'add': v = (v - (imm & MASK64)) & MASK64
        elif op == 'sub': v = (v + (imm & MASK64)) & MASK64
        elif op == 'rol': v = ror64(v, imm & 63)
        elif op == 'ror': v = rol64(v, imm & 63)
        elif op == 'mul': v = (v * pow(imm|1, -1, 1<<64)) & MASK64
        elif op == 'xortmp':
            if tmpA_final is None: raise RuntimeError("xortmp inversion requires tmpA_final")
            v ^= tmpA_final
    return v

def choose_seq(rnd, length, allow_xortmp=False, aggressive=False):
    seq = []
    for i in range(length):
        op = rnd.choice(BASE_OPS)
        if allow_xortmp and rnd.random() < 0.25 and i == length - 1:
            seq.append(('xortmp', 0)); continue
        if op in ('rol', 'ror'):
            imm = rnd.randint(1, 63)
        elif op == 'mul':
            imm = rnd.getrandbits(32) | 1
        else:
            imm = rnd.getrandbits(32 if not aggressive else 48) or 1
        seq.append((op, imm))
    return seq

def seq_to_c_expr(var, seq):
    expr = var
    for op, imm in seq:
        if op == 'xor': expr = f"({expr} ^ 0x{imm:016X}ULL)"
        elif op == 'add': expr = f"({expr} + 0x{imm:016X}ULL)"
        elif op == 'sub': expr = f"({expr} - 0x{imm:016X}ULL)"
        elif op == 'rol':
            n = imm & 63
            expr = f"((( {expr} << {n} ) | ( {expr} >> {64-n} )) & 0xFFFFFFFFFFFFFFFFULL)"
        elif op == 'ror':
            n = imm & 63
            expr = f"((( {expr} >> {n} ) | ( {expr} << {64-n} )) & 0xFFFFFFFFFFFFFFFFULL)"
        elif op == 'mul': expr = f"({expr} * 0x{imm:016X}ULL)"
        elif op == 'xortmp': expr = f"({expr} ^ __hc_tmpA)"
    return expr

def build_block(rnd, tgt, aggressive=False):
    found = False; attempts = 0
    while not found:
        attempts += 1
        seqA = choose_seq(rnd, rnd.choice([2,3,3,4] if not aggressive else [3,4,4,5]), False, aggressive)
        seqB = choose_seq(rnd, rnd.choice([2,3] if not aggressive else [3,4]), True, aggressive)
        tmpA_target = rnd.getrandbits(64)
        E_A = invert_seq(tmpA_target, seqA)
        tmpA_final = apply_seq_forward(E_A, seqA)
        try:
            E_B = invert_seq(tgt, seqB, tmpA_final)
        except Exception:
            continue
        if apply_seq_forward(E_A, seqA) == tmpA_final and apply_seq_forward(E_B, seqB, tmpA_final) == (tgt & MASK64):
            found = True
        if attempts > 8000:
            raise RuntimeError("failed to generate block")
    return E_A, seqA, E_B, seqB

def make_hex_prefix(data: bytes, max_bytes: int = 6) -> str:
    if len(data) == 0:
        return "empty"
    take = min(len(data), max_bytes)
    return data[:take].hex().lower()

def sanitize_identifier(s: str) -> str:
    # Replace invalid characters with underscores
    sanitized = re.sub(r'[^0-9a-zA-Z_]', '_', s)
    # Prepend underscore if it starts with a digit
    if sanitized and sanitized[0].isdigit():
        sanitized = '_' + sanitized
    return sanitized

def make_func_name(data: bytes) -> str:
    try:
        s = data.decode('latin1')
    except Exception:
        s = data.hex()
    sanitized = sanitize_identifier(s)
    return f"decode_{sanitized}__{len(data)}"

def make_macro_name(data: bytes) -> str:
    try:
        s = data.decode('latin1')
    except Exception:
        s = data.hex()
    sanitized = sanitize_identifier(s).upper()
    return f"DECODE_{sanitized}__{len(data)}"

def build_c(input_bytes: bytes, seed: str, aggressive: bool, mode: str):
    rnd = seeded_rng(seed)
    n = len(input_bytes)
    chunk = 8
    num_blocks = (n + chunk - 1) // chunk

    func_name = make_func_name(input_bytes)
    macro_name = make_macro_name(input_bytes)

    out = []
    #out.append("#include <stdint.h>")
    #out.append("#include <stdlib.h>")
    #out.append("#include <stdio.h>")

    if mode == 'func':
        out.append(f"static inline __attribute__((always_inline)) unsigned char* {func_name}(void){{")
        out.append(f"    size_t __hc_len = {n+1}u;")
        out.append("    unsigned char* buf = (unsigned char*)mallocM(__hc_len);")
        out.append("    if(!buf) return NULL;")
        out.append("    volatile unsigned long long __hc_tmpA,__hc_tmpB;")
        for b in range(num_blocks):
            take = min(chunk, n - b*chunk)
            tgt = 0
            for j in range(take):
                tgt |= (input_bytes[b*chunk + j] & 0xFF) << (8*j)
            E_A, seqA, E_B, seqB = build_block(rnd, tgt, aggressive)
            out.append(f"    /* block {b} */")
            out.append(f"    __hc_tmpA = 0x{E_A:016X}ULL; __hc_tmpA = {seq_to_c_expr('__hc_tmpA', seqA)};")
            out.append(f"    __hc_tmpB = 0x{E_B:016X}ULL; __hc_tmpB = {seq_to_c_expr('__hc_tmpB', seqB)};")
            for j in range(take):
                out.append(f"    buf[{b*chunk + j}] = (unsigned char)((__hc_tmpB >> {8*j}) & 0xFFULL);")
        out.append(f"    buf[{n}] = 0x0;")
        out.append("    return buf;")
        out.append("}\n")
        # out.append(f"static inline __attribute__((always_inline)) size_t {func_name}_len(void){{ return (size_t){n}u; }}")

    elif mode == 'macro':
        prefix = sanitize_identifier(input_bytes.decode('latin1', errors='ignore')[:6])
        buf_name = f"__hc_buf_{prefix}"
        len_name = f"__hc_len_{prefix}"
        tmpA_name = "__hc_tmpA"
        tmpB_name = "__hc_tmpB"

        body = []
        body.append(f"size_t {len_name} = {n}u;")
        body.append(f"unsigned char* {buf_name} = (unsigned char*)malloc({len_name});")
        body.append(f"if (!{buf_name}) {{ {buf_name} = NULL; }}")
        body.append(f"volatile unsigned long long {tmpA_name},{tmpB_name};")
        for b in range(num_blocks):
            take = min(chunk, n - b*chunk)
            tgt = 0
            for j in range(take):
                tgt |= (input_bytes[b*chunk + j] & 0xFF) << (8*j)
            E_A, seqA, E_B, seqB = build_block(rnd, tgt, aggressive)
            body.append(f"/* block {b} */")
            body.append(f"{tmpA_name} = 0x{E_A:016X}ULL; {tmpA_name} = {seq_to_c_expr(tmpA_name, seqA)};")
            body.append(f"{tmpB_name} = 0x{E_B:016X}ULL; {tmpB_name} = {seq_to_c_expr(tmpB_name, seqB)};")
            for j in range(take):
                body.append(f"{buf_name}[{b*chunk + j}] = (unsigned char)(({tmpB_name} >> {8*j}) & 0xFFULL);")
        body.append(f"{buf_name};")

        macro_single = "({ " + " ".join(body) + " })"
        out.append(f"#undef {macro_name}")
        out.append(f"#define {macro_name} {macro_single}")
        out.append(f"#define {macro_name}_LEN ({n}u)")

    return "\n".join(out)

def main():
    ap = argparse.ArgumentParser()
    mx = ap.add_mutually_exclusive_group(required=True)
    mx.add_argument("--string", help="literal string")
    mx.add_argument("--hex", help="hex (even length)")
    mx.add_argument("--infile", help="file")
    ap.add_argument("-o","--out", required=True)
    ap.add_argument("--seed", default="seed")
    ap.add_argument("--aggressive", action="store_true")
    ap.add_argument("--mode", choices=['func','macro'], default='func')
    args = ap.parse_args()

    if args.string is not None:
        data = args.string.encode("latin1")
    elif args.hex is not None:
        data = bytes.fromhex(args.hex)
    else:
        with open(args.infile,"rb") as f:
            data = f.read()

    c = build_c(data, args.seed, args.aggressive, args.mode)
    with open(args.out, "w") as f:
        f.write(c)
    print(f"Wrote {args.out} (input {len(data)} bytes, mode {args.mode})")

if __name__ == "__main__":
    main()

