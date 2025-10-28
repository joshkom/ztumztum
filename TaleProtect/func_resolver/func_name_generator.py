XOR_KEY = 0x51  # example key
#strings = ["dlsym","mmap", "munmap", "dladdr", "dl_iterate_phdr", "read", "mprotect", "getpagesize"]

strings = ["pthread_create", "pthread_detach", "malloc", "free"]

def make_c_buffer(name, s, xor_key=XOR_KEY, indent=3, max_line_len=60):
    var_name = f"enc_{name}"
    chars = [f"'{c}' ^ XOR_KEY" for c in s] + ["0x0"]

    lines = []
    line = ""
    for ch in chars:
        if line:
            line += ", "
        line += ch
        if len(line) > max_line_len:
            lines.append(line)
            line = ""
    if line:
        lines.append(line)

    if len(lines) == 1:
        return f"static i8 {var_name}[] = {{{lines[0]}}};"
    else:
        formatted_lines = ",\n".join(" " * indent + l for l in lines)
        return f"static i8 {var_name}[] = {{\n{formatted_lines}\n}};"

for s in strings:
    print(make_c_buffer(s, s))
