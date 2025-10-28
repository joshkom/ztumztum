#include <malloc.h>
#include "unstdstring_compat.h"
#include "unstdchar_compat.h"
#include "unstdmemory_compat.h"

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "_unstdcommon.h"


sizet unstdstring_compat_strlen8(const i8 *restrict const buffer_arg) {
    if (!buffer_arg || !*buffer_arg) {
        return 0;
    }

    sizet len = 0;
    while (buffer_arg[++len]);

    return len;
}

sizet unstdstring_compat_strlenu8(const u8t *restrict const buffer_arg) {
    if (!buffer_arg || !*buffer_arg) {
        return 0;
    }

    sizet len = 0;
    while (buffer_arg[++len]);

    return len;
}


sizet unstdstring_compat_strlenu16(const u16t *const buffer_arg) {
    if (!buffer_arg || !*buffer_arg) {
        return 0;
    }

    sizet len = 0;
    while (((const u16t *) buffer_arg)[++len]);

    return len;
}

bool unstdstring_compat_charcmp8(const i8 f_buffer_arg, const i8 s_buffer_arg) {
    return f_buffer_arg == s_buffer_arg;
}

bool unstdstring_compat_charcmp32(const i32 f_buffer_arg, const i32 s_buffer_arg) {
    return f_buffer_arg == s_buffer_arg;
}

bool unstdstring_compat_charcmpu8(const u8t f_buffer_arg, const u8t s_buffer_arg) {
    return f_buffer_arg == s_buffer_arg;
}

bool unstdstring_compat_charcmpu16(const u16t f_buffer_arg, const u16t s_buffer_arg) {
    return f_buffer_arg == s_buffer_arg;
}

bool unstdstring_compat_charcmpu32(const u32t f_buffer_arg, const u32t s_buffer_arg) {
    return f_buffer_arg == s_buffer_arg;
}

bool unstdstring_compat_strequal8(const i8 *const f_buffer_arg, const i8 *const s_buffer_arg) {
    if (!f_buffer_arg || !s_buffer_arg) {
        return false;
    }

    if (!*f_buffer_arg) {
        return !*s_buffer_arg;
    }

    sizet i = 0;
    for (; f_buffer_arg[i] && s_buffer_arg[i]; i++) {
        if (f_buffer_arg[i] != s_buffer_arg[i]) {
            return false;
        }
    }

    return f_buffer_arg[i] == s_buffer_arg[i];
}

bool unstdstring_compat_strequali8(const i8 *const f_buffer_arg, const i8 *const s_buffer_arg) {
    if (!f_buffer_arg || !s_buffer_arg) {
        return false;
    }

    if (!*f_buffer_arg) {
        return !*s_buffer_arg;
    }

    sizet i = 0;
    for (; f_buffer_arg[i] && s_buffer_arg[i]; i++) {
        if (unstdchar_compat_tolower8(f_buffer_arg[i]) != unstdchar_compat_tolower8(s_buffer_arg[i])) {
            return false;
        }
    }

    return unstdchar_compat_tolower8(f_buffer_arg[i]) == unstdchar_compat_tolower8(s_buffer_arg[i]);
}

#define ALIGN (sizeof(size_t))
#define ONES ((size_t)-1 / UCHAR_MAX)
#define HIGHS (ONES * (UCHAR_MAX / 2 + 1))
#define HASZERO(x) (((x)-ONES) & ~(x)&HIGHS)

i8 *__strchrnul(const char *s, int c) {
    const size_t *w;
    size_t k;
    c = (unsigned char) c;

    if (!c) {
        return (char *) (uintptr_t) s + unstdstring_compat_strlen8(s);
    }

    for (; (uintptr_t) s % ALIGN; s++) {
        if (!*s || *(const unsigned char *) s == c) {
            return (char *) (uintptr_t) s;
        }
    }

    k = ONES * (unsigned long) c;

    for (w = (const void *) s; !HASZERO(*w) && !HASZERO(*w ^ k); w++) { ;
    }
    for (s = (const void *) w; *s && *(const unsigned char *) s != c; s++) { ;
    }

    return (char *) (uintptr_t) s;
}

i8 *unstdstring_compat_strchr(const i8 *buffer_arg, i32 char_arg) {
    char *r = __strchrnul(buffer_arg, char_arg);
    return *(unsigned char *) r == (unsigned char) char_arg ? r : 0;
}

i8 *unstdstring_compat_strstr(const i8 *buffer_arg, const i8 *const substring_arg) {
    const i8 *a;
    const i8 *b;

    /* First scan quickly through the two strings looking for a
     * single-character match.  When it's found, then compare the
     * rest of the substring_arg.
     */

    b = substring_arg;

    if (*b == 0) {
        return (i8 *) (uptrt) buffer_arg;
    }

    for (; *buffer_arg != 0; buffer_arg += 1) {
        if (*buffer_arg != *b) {
            continue;
        }

        a = buffer_arg;

        while (1) {
            if (*b == 0) {
                return (i8 *) (uptrt) buffer_arg;
            }

            if (*a++ != *b++) {
                break;
            }
        }

        b = substring_arg;
    }

    return NULL;
}

i8 *unstdstring_compat_strcasestr(const i8 *buffer_arg, const i8 *const substring_arg) {
    const i8 *a;
    const i8 *b;

    b = substring_arg;

    if (*b == 0) {
        return (i8 *) (uptrt) buffer_arg;
    }

    for (; *buffer_arg != 0; buffer_arg += 1) {
        if (unstdchar_compat_tolower8(*buffer_arg) != unstdchar_compat_tolower8(*b)) {
            continue;
        }

        a = buffer_arg;

        while (1) {
            if (*b == 0) {
                return (i8 *) (uptrt) buffer_arg;
            }

            if (unstdchar_compat_tolower8(*a++) != unstdchar_compat_tolower8(*b++)) {
                break;
            }
        }

        b = substring_arg;
    }

    return NULL;
}

i8 *unstdstring_compat_strcat(const i8 *const f_buffer_arg, const i8 *const s_buffer_arg) {
    bool f_empty = (!f_buffer_arg || *f_buffer_arg == 0);
    bool s_empty = (!s_buffer_arg || *s_buffer_arg == 0);

    if (f_empty && s_empty) {
        return NULL;
    }

    if (f_empty) {
        return (i8 *) s_buffer_arg;
    }

    if (s_empty) {
        return (i8 *) f_buffer_arg;
    }

    sizet f_len = unstdstring_compat_strlen8(f_buffer_arg);
    sizet s_len = unstdstring_compat_strlen8(s_buffer_arg);

    i8 *concatenated_str = (i8 *) malloc(f_len + s_len + 1);
    if (!concatenated_str) {
        return NULL;
    }

    unstdmemory_compat_memcpy(concatenated_str, f_buffer_arg, f_len);
    unstdmemory_compat_memcpy(concatenated_str + f_len, s_buffer_arg, s_len);
    concatenated_str[f_len + s_len] = 0;

    return concatenated_str;
}

i32 unstdstring_compat_strcmp(const i8 *const f_buffer_arg, const i8 *const s_buffer_arg) {
    i32 r = -1;

    if (f_buffer_arg == s_buffer_arg) {
        return 0;
    }

    if (f_buffer_arg && s_buffer_arg) {
        sizet i = 0;
        while (f_buffer_arg[i] == s_buffer_arg[i]) {
            if (!f_buffer_arg[i]) {
                r = 0;
                break;
            }
            i++;
        }

        if (r != 0) {
            r = f_buffer_arg[i] - s_buffer_arg[i];
        }
    }

    return r;
}

i32 unstdstring_compat_strncmp(const i8 *const f_buffer_arg,
                               const i8 *const s_buffer_arg,
                               const sizet n_arg) {
    if (!f_buffer_arg || !s_buffer_arg) {
        return (f_buffer_arg == s_buffer_arg) ? 0 : (f_buffer_arg ? 1 : -1);
    }

    for (sizet i = 0; i < n_arg; i++) {
        unsigned char fc = f_buffer_arg[i];
        unsigned char sc = s_buffer_arg[i];

        if (fc != sc || fc == '\0' || sc == '\0') {
            return (i32) (fc - sc);
        }
    }

    return 0;
}
