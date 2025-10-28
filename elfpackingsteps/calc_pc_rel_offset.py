#!/usr/bin/env python3

def calculate_aarch64_offsets(pc, target):
    """
    Calculate the offsets for:
        adrp x16, <target_page_relative>
        ldr  x17, [x16, #page_offset]

    Args:
        pc (int): current instruction address
        target (int): target function or GOT entry address
    Returns:
        dict: {'adrp_offset': int, 'ldr_offset': int}
    """

    page_size = 0x1000

    # Page-aligned addresses
    pc_page = pc & ~(page_size - 1)
    target_page = target & ~(page_size - 1)

    # ADRP immediate: difference in pages
    page_diff = target_page - pc_page

    # LDR offset: offset within the page
    page_offset = target & (page_size - 1)

    return {
        'adrp_offset': page_diff,
        'ldr_offset': page_offset
    }


if __name__ == "__main__":
    # Example usage
    pc = int(input("Enter current instruction address (hex): "), 16)
    target = int(input("Enter target address (hex): "), 16)

    offsets = calculate_aarch64_offsets(pc, target)

    print(f"ADR offset (PC-relative page diff): {offsets['adrp_offset']:#x}")
    print(f"LDR offset (within page): {offsets['ldr_offset']:#x}")

    print("\nExample assembly:")
    print(f"adrp x16, #{offsets['adrp_offset']:#x}")
    print(f"ldr  x17, [x16, #{offsets['ldr_offset']:#x}]")
    print("br   x17  # or blr x17 if you need to return")

