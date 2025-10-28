import re

def generate_wrappers(typedefs_text):
    # Match typedef lines
    typedef_pattern = re.compile(r'typedef\s+(.+?\(\s*\*([a-zA-Z0-9_]+)_t\)\s*\([^)]*\));', re.DOTALL)
    
    typedefs = typedef_pattern.findall(typedefs_text)
    
    result = []
    for i, (full_typedef, name) in enumerate(typedefs):
        # Clean up whitespace inside parentheses
        full_typedef_clean = re.sub(r'\s+', ' ', full_typedef)
        # Add numbered comment
        typedef_line = f"{full_typedef}                                                                   /* {i} */"
        # Add #define line
        define_line = f"#define {name}M funcresolver_3000_wrapfunc_typedM({i}, {name}_t)"
        result.append(typedef_line)
        result.append(define_line)
        result.append("")  # blank line
    
    return "\n".join(result)

# Example usage
input_text = """
typedef void *_Nullable (*fr3_dlsym_t)(
        void *__BIONIC_COMPLICATED_NULLNESS __handle,
        const char *_Nullable __symbol
);
typedef void *_Nullable (*fr3_mmap_t)(
        void *_Nullable __addr,
        size_t __size,
        int __prot,
        int __flags,
        int __fd,
        off_t __offset
);
"""

print(generate_wrappers(input_text))

