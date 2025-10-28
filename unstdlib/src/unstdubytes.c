#include "unstdubytes.h"
#include <malloc.h>
#include <string.h>

u8t unstdubytes_grow(ubytes *const buffer_arg, const sizet size_bytes_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (!size_bytes_arg) {
        return 3;
    }

    void *realloc_result = realloc(buffer_arg->data, buffer_arg->capacity + size_bytes_arg);
    if (!realloc_result) {
        return 4;
    }

    buffer_arg->data = realloc_result;
    buffer_arg->capacity = buffer_arg->capacity + size_bytes_arg;

    return 1;
}

u8t unstdubytes_writebytes(ubytes *const destination_buffer_arg,
                           const u8t *const source_buffer_arg,
                           const sizet source_buffer_size_arg,
                           const sizet offset_arg) {
    if (!destination_buffer_arg) {
        return 2;
    }

    if (!source_buffer_arg) {
        return 3;
    }

    if (!source_buffer_size_arg) {
        return 4;
    }

    if (offset_arg + source_buffer_size_arg > destination_buffer_arg->capacity) {
        return 5;
    }

    memcpy(destination_buffer_arg->data + offset_arg,
           source_buffer_arg,
           source_buffer_size_arg
    );

    destination_buffer_arg->length = offset_arg + source_buffer_size_arg;

    return 1;
}

u8t unstdubytes_appendbytes(ubytes *const destination_buffer_arg,
                            const u8t *const source_buffer_arg,
                            const sizet source_buffer_size_arg) {
    if (!destination_buffer_arg) {
        return 2;
    }

    if (!source_buffer_arg) {
        return 3;
    }

    if (!source_buffer_size_arg) {
        return 4;
    }

    if (source_buffer_size_arg + destination_buffer_arg->length > destination_buffer_arg->capacity) {
        if (unstdubytes_grow(
                destination_buffer_arg,
                source_buffer_size_arg + destination_buffer_arg->length - destination_buffer_arg->capacity
        ) != 1) {
            return 5;
        }
    }

    memcpy(destination_buffer_arg->data + destination_buffer_arg->length,
           source_buffer_arg,
           source_buffer_size_arg
    );

    destination_buffer_arg->length += source_buffer_size_arg;

    return 1;
}

u8t unstdubytes_writebytes_autogrow_mode(ubytes *const destination_buffer_arg,
                                         const u8t *const source_buffer_arg,
                                         const sizet source_buffer_size_arg,
                                         const sizet offset_arg,
                                         unstdubytes_growth_mode growth_mode_arg) {
    if (!destination_buffer_arg) {
        return 2;
    }

    if (!source_buffer_arg) {
        return 3;
    }

    if (!source_buffer_size_arg) {
        return 4;
    }

    sizet required_size = offset_arg + source_buffer_size_arg;

    if (required_size > destination_buffer_arg->capacity) {
        sizet grow_size;

        if (growth_mode_arg == UNSTDUBYTES_GROWTH_EXPONENTIAL) {
            sizet new_capacity = destination_buffer_arg->capacity
                                 ? destination_buffer_arg->capacity
                                 : 1;
            while (new_capacity < required_size) { new_capacity *= 2; }
            grow_size = new_capacity - destination_buffer_arg->capacity;
        } else {
            grow_size = required_size - destination_buffer_arg->capacity;
        }

        u8t grow_result = unstdubytes_grow(destination_buffer_arg, grow_size);
        if (grow_result != 1) {
            return 10 + grow_result;
        }
    }

    return unstdubytes_writebytes(
            destination_buffer_arg,
            source_buffer_arg,
            source_buffer_size_arg,
            offset_arg
    );
}

u8t unstdubytes_writebytes_autogrow_linear(ubytes *const destination_buffer_arg,
                                           const u8t *const source_buffer_arg,
                                           const sizet source_buffer_size_arg,
                                           const sizet offset_arg) {
    return unstdubytes_writebytes_autogrow_mode(
            destination_buffer_arg,
            source_buffer_arg,
            source_buffer_size_arg,
            offset_arg,
            UNSTDUBYTES_GROWTH_LINEAR
    );
}

sizet unstdubytes_length(ubytes *buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        if (out_error_arg) { *out_error_arg = 2; }
        return 0;
    }

    if (out_error_arg) { *out_error_arg = 1; }

    return buffer_arg->length;
}

bool unstdubytes_set_length(ubytes *buffer_arg, sizet new_length_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        if (out_error_arg) { *out_error_arg = 2; }
        return false;
    }

    if (new_length_arg > buffer_arg->capacity) {
        if (out_error_arg) { *out_error_arg = 3; }
        return false;
    }

    buffer_arg->length = new_length_arg;

    if (out_error_arg) { *out_error_arg = 1; }

    return true;
}

bool unstdubytes_increase_length(ubytes *buffer_arg, sizet amount_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        if (out_error_arg) { *out_error_arg = 2; }
        return false;
    }

    if (buffer_arg->length + amount_arg > buffer_arg->capacity) {
        if (out_error_arg) { *out_error_arg = 3; }
        return false;
    }

    buffer_arg->length += amount_arg;

    if (out_error_arg) { *out_error_arg = 1; }

    return true;
}


sizet unstdubytes_remaining(ubytes *buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        if (out_error_arg) { *out_error_arg = 2; }
        return 0;
    }

    if (out_error_arg) { *out_error_arg = 1; }

    return buffer_arg->capacity - buffer_arg->length;
}

bool unstdubytes_has_remaining(ubytes *buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        if (out_error_arg) {
            *out_error_arg = 2;
        }
        return 0;
    }

    if (out_error_arg) {
        *out_error_arg = 1;
    }

    return buffer_arg->capacity > buffer_arg->length;
}
