#include "unstdbitutils.h"

bool unstdbitutils_fitsinbits(u32t value_arg, u8t bits_arg) {
    return value_arg <= ((1U << bits_arg) - 1);
}

//! [8-bit]
u8t unstdbitutils_packltrmulti8(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t packet_buffer_arg = 0;
    u8t shift = sizeof(u8t) * 8;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u8t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        shift -= bits;
        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}


u8t unstdbitutils_packrtlmulti8(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t packet_buffer_arg = 0;
    u8t shift = 0;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u8t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u8t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits((u8t) value, (u8t) bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

void unstdbitutils_unpackltrmulti8(u8t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = sizeof(u8t) * 8;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u8t *value = va_arg(args, u8t *);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> (shift - bits)) & (1U << bits) - 1;
        }

        shift -= bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

void unstdbitutils_unpackrtlmulti8(u8t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = 0;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u8t *value = va_arg(args, u8t *);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u8t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> shift) & (1U << bits) - 1;
        }

        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

//! [16-bit]
u16t unstdbitutils_packltrmulti16(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u16t packet_buffer_arg = 0;
    u8t shift = sizeof(u16t) * 8;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u16t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        shift -= bits;
        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

u16t unstdbitutils_packrtlmulti16(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u16t packet_buffer_arg = 0;
    u8t shift = 0;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u16t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u16t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

void unstdbitutils_unpackltrmulti16(u16t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = sizeof(u16t) * 8;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u16t *value = va_arg(args, u16t *);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> (shift - bits)) & ((1U << bits) - 1);
        }

        shift -= bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

void unstdbitutils_unpackrtlmulti16(u16t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = 0;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u16t *value = va_arg(args, u16t *);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u16t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> shift) & ((1U << bits) - 1);
        }

        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

//! [32-bit]
u32t unstdbitutils_packltrmulti32(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u32t packet_buffer_arg = 0;
    u8t shift = sizeof(u32t) * 8;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u32t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        shift -= bits;
        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

u32t unstdbitutils_packrtlmulti32(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u32t packet_buffer_arg = 0;
    u8t shift = 0;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u32t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u32t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        packet_buffer_arg |= (value & ((1U << bits) - 1)) << shift;
        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

void unstdbitutils_unpackltrmulti32(u32t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = sizeof(u32t) * 8;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u32t *value = va_arg(args, u32t *);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> (shift - bits)) & ((1U << bits) - 1);
        }

        shift -= bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

void unstdbitutils_unpackrtlmulti32(u32t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = 0;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u32t *value = va_arg(args, u32t *);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u32t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> shift) & ((1U << bits) - 1);
        }

        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

//! [64-bit]
u64t unstdbitutils_packltrmulti64(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u64t packet_buffer_arg = 0;
    u8t shift = sizeof(u64t) * 8;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u64t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        shift -= bits;
        packet_buffer_arg |= (value & ((1ULL << bits) - 1)) << shift;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

u64t unstdbitutils_packrtlmulti64(u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u64t packet_buffer_arg = 0;
    u8t shift = 0;

    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u64t value = va_arg(args, u32t);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u64t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return 0;
        }

        if (!unstdbitutils_fitsinbits(value, bits)) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 3;
            }
            return 0;
        }

        packet_buffer_arg |= (value & ((1ULL << bits) - 1)) << shift;
        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return packet_buffer_arg;
}

void unstdbitutils_unpackltrmulti64(u64t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = sizeof(u64t) * 8;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u64t *value = va_arg(args, u64t *);

        if (!bits) {
            continue;
        }

        if (bits > shift) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> (shift - bits)) & ((1ULL << bits) - 1);
        }

        shift -= bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}

void unstdbitutils_unpackrtlmulti64(u64t packet_buffer_arg, u8t pairs_count_arg, u8t *const out_error_arg, ...) {
    va_list args;
    va_start(args, out_error_arg);

    u8t shift = 0;
    for (u8t i = 0; i < pairs_count_arg; ++i) {
        u8t bits = va_arg(args, u32t);
        u64t *value = va_arg(args, u64t *);

        if (!bits) {
            continue;
        }

        if (shift + bits > sizeof(u64t) * 8) {
            va_end(args);
            if (out_error_arg) {
                *out_error_arg = 2;
            }
            return;
        }

        if (value) {
            *value = (packet_buffer_arg >> shift) & ((1ULL << bits) - 1);
        }

        shift += bits;
    }

    va_end(args);

    if (out_error_arg) {
        *out_error_arg = 1;
    }
}
