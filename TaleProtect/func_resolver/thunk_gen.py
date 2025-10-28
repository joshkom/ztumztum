#!/usr/bin/env python3
"""
Generate encoded name buffers, typedef wrappers and libc_funcs[] array
from C function-pointer typedefs.

- Drops a leading `fr3_` from func names when producing enc_ and define names
  (so `fr3_dlsym_t` -> `enc_dlsym` and `#define dlsymM ...`).
- Preserves typedef block formatting, appends numbered comment `/* n */`.
- Keeps order of first appearance; duplicate typedefs are ignored after the first.
"""

import re
import sys
from textwrap import indent

# Use the XOR key present in your C code (example: 0xf1)
XOR_KEY = 0xf1

# tweak formatting behavior here:
INDENT = 8
MAX_LINE_LEN = 70  # approx char length per line for listing char entries


def find_typedefs(text):
    """
    Return list of tuples (full_typedef_block, base_name_without__t) in order of appearance.
    Only matches typedefs that are function-pointer style: typedef <ret> (*name_t)(...);
    """
    # Non-greedy match for the whole typedef up to the terminating semicolon.
    pattern = re.compile(
        r"(typedef\s+[\s\S]*?\(\s*\*([A-Za-z0-9_]+)_t\)\s*\([^;]*\)\s*;)",
        re.MULTILINE,
    )
    matches = pattern.findall(text)
    return matches  # list of (full_block, base_name)


def clean_name(base_name):
    """Remove leading fr3_ if present for cleaner enc/define names."""
    if base_name.startswith("fr3_"):
        return base_name[4:]
    return base_name


def make_c_buffer(funcname, xor_key=XOR_KEY, indent_spaces=INDENT, max_line_len=MAX_LINE_LEN):
    """
    Create a C static i8 enc_<funcname>[] initializer using 'c' ^ FR3_XOR_KEY entries and trailing 0x0.
    Tries to wrap lines to stay under max_line_len.
    """
    var_name = f"enc_{funcname}"
    chars = [f"'{c}' ^ FR3_XOR_KEY" for c in funcname] + ["0x0"]
    lines = []
    cur = ""
    for ch in chars:
        part = (", " if cur else "") + ch
        if len(cur) + len(part) > max_line_len and cur:
            lines.append(cur)
            cur = ch
        else:
            cur += part
    if cur:
        lines.append(cur)

    if len(lines) == 1:
        return f"static i8 {var_name}[] = {{{lines[0]}}};"
    else:
        body = ",\n".join((" " * indent_spaces) + l for l in lines)
        return f"static i8 {var_name}[] = {{\n{body}\n}};"


def generate_outputs(typedef_text):
    matches = find_typedefs(typedef_text)
    seen = set()
    ordered = []  # list of dicts with keys: idx, typedef_block, base_name, clean_name

    idx = 0
    for full_block, base_name in matches:
        if base_name in seen:
            continue
        seen.add(base_name)
        c_name = clean_name(base_name)
        ordered.append(
            {
                "idx": idx,
                "typedef_block": full_block.strip(),
                "base_name": base_name,  # e.g. fr3_dlsym
                "clean_name": c_name,  # e.g. dlsym
            }
        )
        idx += 1

    # Build encoded buffers
    buffers = []
    for entry in ordered:
        buffers.append(make_c_buffer(entry["clean_name"]))

    # Build typedef wrappers
    typedef_lines = []
    for entry in ordered:
        # Keep original typedef formatting, append comment with index
        # Ensure typedef ends with semicolon in the block; we append comment after the semicolon.
        block = entry["typedef_block"]
        # place comment on same line after semicolon: we will reconstruct block + space + comment
        typedef_lines.append(f"{block}                                                                   /* {entry['idx']} */")
        # define clean_nameM -> funcresolver_3000_wrapfunc_typedM(idx, base_name_t)
        typedef_lines.append(f"#define {entry['clean_name']}M funcresolver_3000_wrapfunc_typedM({entry['idx']}, {entry['base_name']}_t)")
        typedef_lines.append("")  # blank line

    # Build libc_funcs array lines in same order:
    libc_entries = [f"        funcresolver_3000_populate_funcM(enc_{e['clean_name']})," for e in ordered]

    # Compose final C output
    out_parts = []

    # XOR key and buffers
    # out_parts.append(f"#define XOR_KEY 0x{XOR_KEY:02x}\n")
    out_parts.append("// --- Encoded function name buffers ---\n")
    out_parts.append("\n\n".join(buffers))
    out_parts.append("\n")

    # helper macro (from your snippet) - keep exact macro form as provided
    #out_parts.append(
    #    '/* helper macro from your code (keeps the same fields you had) */\n'
    #    '#define funcresolver_3000_populate_funcM(enc_name_ptr_arg) {enc_name_ptr_arg, sizeof(enc_name_ptr_arg)/sizeof(enc_name_ptr_arg[0]), NULL, {0}}\n'
    #)

    # libc_funcs array
    out_parts.append("libc_func_entry_t libc_funcs[] = {")
    out_parts.append("\n".join(libc_entries))
    out_parts.append("};\n")

    # typedef wrappers
    out_parts.append("\n// --- Typedef wrappers ---\n")
    out_parts.append("\n".join(typedef_lines))

    return "\n".join(out_parts)


if __name__ == "__main__":
    # Example: replace input_typedefs with reading a file or stdin if you prefer
    input_typedefs = """
typedef void *_Nullable (*fr3_dlsym_t)(
        void *__BIONIC_COMPLICATED_NULLNESS __handle,
        const char *_Nullable __symbol
);

typedef void *_Nullable (*fr3_malloc_t)(
        size_t __byte_count
);                                                       
typedef void *_Nullable (*fr3_calloc_t)(
        size_t __item_count,
        size_t __item_size
);
typedef void *_Nullable (*fr3_realloc_t)(
        void *_Nullable __ptr,
        size_t __byte_count
);
typedef void (*fr3_free_t)(
        void *_Nullable __ptr
); 

typedef pid_t (*fr3_fork_t)(
        void
);

typedef int (*fr3_mprotect_t)(
        void *_Nonnull __addr,
        size_t __size,
        int __prot
);
typedef void *_Nullable (*fr3_mmap_t)(
        void *_Nullable __addr,
        size_t __size,
        int __prot,
        int __flags,
        int __fd,
        off_t __offset
);
typedef int   (*fr3_munmap_t)(
        void *_Nonnull,
        sizet
); 

typedef int   (*fr3_dladdr_t)(
        const void *_Nonnull,
        Dl_info *_Nonnull
);
typedef int (*fr3_dl_iterate_phdr_t)(
        int (*_Nonnull callback)(struct dl_phdr_info *_Nonnull, size_t, void *_Nullable),
        void *_Nullable data
);

typedef ssize_t (*fr3_read_t)(
        int fd,
        void *_Nonnull const __pass_object_size0 buf,
        size_t count
);

typedef int (*fr3_getpagesize_t)(
        void
);
typedef long (*fr3_sysconf_t)(
        int __name
);

typedef int (*fr3_pthread_create_t)(
        pthread_t *_Nonnull __pthread_ptr,
        pthread_attr_t const *_Nullable __attr,
        void *_Nonnull (*_Nonnull __start_routine)(void *_Nonnull),
        void *_Nullable
);
typedef int (*fr3_pthread_detach_t)(
        pthread_t __pthread
);
typedef int (*fr3_pthread_once_t)(
        pthread_once_t *_Nonnull __once,
        void (*_Nonnull __init_routine)(void)
);

typedef int (*fr3___system_property_get_t)(
        const char *_Nonnull __name,
        char *_Nonnull __value
);
typedef char *_Nullable (*fr3_getenv_t)(
        const char *_Nonnull __name
);

typedef int (*fr3_sem_init_t)(
        sem_t *_Nonnull __sem,
        int __shared,
        unsigned int __value
);
typedef int (*fr3_sem_post_t)(
        sem_t *_Nonnull __sem
);
typedef int (*fr3_sem_wait_t)(
        sem_t *_Nonnull __sem
);
typedef int (*fr3_sem_timedwait_t)(
        sem_t *_Nonnull __sem,
        const struct timespec *_Nonnull __ts
);
typedef int (*fr3_sem_trywait_t)(
        sem_t *_Nonnull __sem
);

"""

    # If a filename arg provided, read it instead of using the sample
    if len(sys.argv) > 1:
        with open(sys.argv[1], "r", encoding="utf-8") as f:
            input_typedefs = f.read()

    print(generate_outputs(input_typedefs))

