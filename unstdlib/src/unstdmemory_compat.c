#include "unstdmemory_compat.h"


u8t *unstdmemory_compat_memset(void *const buffer_arg, i32 char_arg, sizet size_arg) {
    u8t *s = buffer_arg;
    sizet k;

    if (!size_arg) {
        return buffer_arg;
    }

    s[0] = s[size_arg - 1] = (u8t) char_arg;

    if (size_arg <= 2) {
        return buffer_arg;
    }

    s[1] = s[size_arg - 2] = (u8t) char_arg;
    s[2] = s[size_arg - 3] = (u8t) char_arg;

    if (size_arg <= 6) {
        return buffer_arg;
    }

    s[3] = s[size_arg - 4] = (u8t) char_arg;

    if (size_arg <= 8) {
        return buffer_arg;
    }

    k = -(uptrt) s & 3;
    s += k;
    size_arg -= k;
    size_arg &= (u32l) -4;
    size_arg /= 4;

    u32t *ws = (u32t *) (void *) s;
    u32t wc = char_arg & 0xFF;
    wc |= ((wc << 8) | (wc << 16) | (wc << 24));

    for (; size_arg; size_arg--, ws++) {
        *ws = wc;
    }

    return buffer_arg;
}


i32 unstdmemory_compat_memcmp(const void *const f_buffer_arg,
                              const void *const s_buffer_arg,
                              sizet size_arg) {
    const u8t *s1 = f_buffer_arg;
    const u8t *s2 = s_buffer_arg;
    sizet i;

    if (s1 == s2) {
        return 0;
    }

    if (!s1) {
        return 1;
    }
    if (!s2) {
        return -1;
    }

    for (i = 0; i < size_arg; i++, s1++, s2++) {
        if (*s1 != *s2) {
            return *s1 - *s2;
        }
    }

    return 0;
}

#define wsize sizeof(i32)
#define wmask (wsize - 1)

void *unstdmemory_compat_memcpy(void *const dst_arg, const void *const src_arg, sizet length_arg) {
    u8t *dst = dst_arg;
    const u8t *src = src_arg;
    sizet t;

    /* quick exit for zero-length copy or identical poi32ers */
    if (!length_arg || dst == src) {
        return dst_arg;
    }

    /* forward copy if destination is before source */
    if ((uptrt) dst < (uptrt) src) {
        /* try to align operands */
        t = (uptrt) src;
        if ((t | (uptrt) dst) & wmask) {
            if ((t ^ (uptrt) dst) & wmask || length_arg < wsize) {
                t = length_arg;
            } else {
                t = wsize - (t & wmask);
            }

            length_arg -= t;
            for (sizet i = 0; i < t; i++) {
                *dst++ = *src++;
            }
        }

        /* copy whole words */
        t = length_arg / wsize;
        for (sizet i = 0; i < t; i++) {
            *(i32 *) (void *) dst = *(const i32 *) (const void *) src;
            src += wsize;
            dst += wsize;
        }

        /* copy remaining bytes */
        t = length_arg & wmask;
        for (sizet i = 0; i < t; i++) {
            *dst++ = *src++;
        }
    } else {
        /* backward copy if destination is after source */
        src += length_arg;
        dst += length_arg;

        /* align operands */
        t = (uptrt) src;
        if ((t | (uptrt) dst) & wmask) {
            if ((t ^ (uptrt) dst) & wmask || length_arg <= wsize) {
                t = length_arg;
            } else {
                t &= wmask;
            }

            length_arg -= t;
            for (sizet i = 0; i < t; i++) {
                *--dst = *--src;
            }
        }

        /* copy whole words backward */
        t = length_arg / wsize;
        for (sizet i = 0; i < t; i++) {
            src -= wsize;
            dst -= wsize;
            *(i32 *) (void *) dst = *(const i32 *) (const void *) src;
        }

        /* copy remaining bytes backward */
        t = length_arg & wmask;
        for (sizet i = 0; i < t; i++) {
            *--dst = *--src;
        }
    }

    return dst_arg;
}
