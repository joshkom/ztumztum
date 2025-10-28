#include "unstdmemory.h"
#include "_unstdcommon.h"

#include <stdlib.h>
#include <memory.h>

sizet unstdmemory_findbytesindex(const u8t *haystack_arg,
                                 sizet haystack_length_arg,
                                 const u8t *needle_arg,
                                 sizet needle_length_arg,
                                 sizet position_arg,
                                 u8t *const out_error_arg) {
    if (!haystack_length_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!needle_length_arg) {
        unstdcommon_returnM(3) (position_arg <= haystack_length_arg) ? position_arg : 0;
    }

    if (position_arg >= haystack_length_arg) {
        unstdcommon_returnM(4) 0;
    }

    if (needle_length_arg + position_arg > haystack_length_arg) {
        unstdcommon_returnM(5) 0;
    }

    u8t first_byte = needle_arg[0];
    const u8t *first = haystack_arg + position_arg;
    const u8t *last = haystack_arg + haystack_length_arg;
    sizet remaining_len = haystack_length_arg - position_arg;

    while (remaining_len >= needle_length_arg) {
        first = memchr(first, first_byte, remaining_len - needle_length_arg + 1);
        if (!first) {
            unstdcommon_returnM(6) 0;
        }

        if (memcmp(first, needle_arg, needle_length_arg) == 0) {
            unstdcommon_returnM(1) first - haystack_arg;
        }

        remaining_len = last - ++first;
    }

    unstdcommon_returnM(6) 0;
}

u8t unstdmemory_erasebytes(u8t *buffer_arg, sizet buffer_length_arg, size_t position_arg, sizet bytes_number_arg) {
    if (position_arg >= buffer_length_arg) {
        return 2;
    }

    if (!bytes_number_arg) {
        return 3;
    }

    if (bytes_number_arg == (sizet) -1 || position_arg + bytes_number_arg > buffer_length_arg) {
        bytes_number_arg = buffer_length_arg - position_arg;
    }

    sizet remaining_bytes = buffer_length_arg - position_arg - bytes_number_arg;

    if (remaining_bytes > 0) {
        memmove(buffer_arg + position_arg, buffer_arg + position_arg + bytes_number_arg, remaining_bytes);
    }

    return 1;
}
