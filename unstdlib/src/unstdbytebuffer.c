#include "unstdbytebuffer.h"
#include "_unstdcommon.h"
#include <malloc.h>
#include <string.h>


unstdbytebuffer *unstdbytebuffer_new(const sizet preallocate_size_arg,
                                     const sizet max_capacity_arg,
                                     u8t *const out_error_arg) {
    unstdbytebuffer *l_bytebuffer = (unstdbytebuffer *) malloc(sizeof(unstdbytebuffer));

    if (!l_bytebuffer) {
        unstdcommon_returnM(2) NULL;
    }

    l_bytebuffer->position = 0;
    l_bytebuffer->limit = max_capacity_arg;
//    l_bytebuffer->calculate_size_only = false;
//    l_bytebuffer->sliced = false;
//    l_bytebuffer->buffer_owner = true;
    l_bytebuffer->buffer.length = 0;
    l_bytebuffer->buffer.capacity = max_capacity_arg > 0 && preallocate_size_arg > max_capacity_arg
                                    ? max_capacity_arg : preallocate_size_arg;
    l_bytebuffer->buffer.data = NULL;

    if (preallocate_size_arg > 0) {
        l_bytebuffer->buffer.data = (u8t *) malloc(l_bytebuffer->buffer.capacity);
        if (!l_bytebuffer->buffer.data) {
            free(l_bytebuffer);
            unstdcommon_returnM(3) NULL;
        }
    }

    unstdcommon_returnM(1) l_bytebuffer;
}

unstdbytebuffer *unstdbytebuffer_new_overload_0(const sizet size_arg, u8t *const out_error_arg) {
    return unstdbytebuffer_new(size_arg, size_arg, out_error_arg);
}

unstdbytebuffer *unstdbytebuffer_new_from_data(u8t *buffer_arg, sizet length_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (length_arg <= 0) {
        unstdcommon_returnM(3) NULL;
    }

    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(0, length_arg, &err);
    if (!bb || err != 1) {
        unstdcommon_returnM(4) NULL;
    }

    bb->buffer.data = buffer_arg;
    bb->buffer.capacity = length_arg;
    bb->buffer.length = length_arg;
    bb->limit = length_arg;

    unstdcommon_returnM(1) bb;
}

unstdbytebuffer *unstdbytebuffer_new_from_data_copy(u8t *buffer_arg, sizet length_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (length_arg <= 0) {
        unstdcommon_returnM(3) NULL;
    }

    u8t err = 0;
    unstdbytebuffer *bb = unstdbytebuffer_new(length_arg, length_arg, &err);
    if (!bb || err != 1) {
        unstdcommon_returnM(4) NULL;
    }

    memcpy(bb->buffer.data, buffer_arg, length_arg);
    bb->buffer.length = length_arg;

    unstdcommon_returnM(1) bb;
}

u8t unstdbytebuffer_evaluatefit(const unstdbytebuffer *const buffer_arg, const sizet size_bytes_arg) {
    if (buffer_arg->limit && buffer_arg->buffer.length + size_bytes_arg > buffer_arg->limit) {
        return 0;
    }

    if (buffer_arg->buffer.length + size_bytes_arg > buffer_arg->buffer.capacity) {
        return 2;
    }

    return 1;
}

static void unstdbytebuffer_write_bytes_internal(unstdbytebuffer *context_arg,
                                                 const u8t *buffer_arg,
                                                 sizet offset_arg,
                                                 sizet length_arg) {
    memcpy(context_arg->buffer.data + context_arg->position, buffer_arg + offset_arg, length_arg);
    context_arg->position += length_arg;
}

void unstdbytebuffer_destroy(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (context_arg->buffer.data) {
        free(context_arg->buffer.data);
        context_arg->buffer.data = NULL;
    }

    free(context_arg);

    unstdcommon_returnM(1);
}

sizet unstdbytebuffer_position(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    unstdcommon_returnM(1) context_arg->position;
}

void unstdbytebuffer_set_position(unstdbytebuffer *context_arg, sizet position_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3);
    }

    if (context_arg->limit && position_arg > context_arg->limit) {
        unstdcommon_returnM(4);
    }

    if (position_arg > context_arg->buffer.capacity) {
        unstdcommon_returnM(5);
    }

    context_arg->position = position_arg;

    unstdcommon_returnM(1);
}

sizet unstdbytebuffer_limit(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    unstdcommon_returnM(1) context_arg->limit;
}

void unstdbytebuffer_set_limit(unstdbytebuffer *context_arg, sizet limit_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3);
    }

    if (limit_arg < context_arg->buffer.capacity) {
        unstdcommon_returnM(4);
    }

    context_arg->limit = limit_arg;

    unstdcommon_returnM(1);
}

sizet unstdbytebuffer_remaining(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    unstdcommon_returnM(1) context_arg->limit - context_arg->position;
}

bool unstdbytebuffer_has_remaining(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    unstdcommon_returnM(1) context_arg->limit ? context_arg->position < context_arg->limit : true;
}

sizet unstdbytebuffer_length(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0;
    }

    unstdcommon_returnM(1) context_arg->buffer.length;
}

u8t *unstdbytebuffer_bytes(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0;
    }

    unstdcommon_returnM(1) context_arg->buffer.data;
}

void unstdbytebuffer_rewind(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    context_arg->position = 0;

    unstdcommon_returnM(1);
}

void unstdbytebuffer_compact(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3);
    }

    if (context_arg->position > context_arg->buffer.length) {
        unstdcommon_returnM(4);
    }

    sizet remaining = context_arg->buffer.length - context_arg->position;
    memmove(context_arg->buffer.data, context_arg->buffer.data + context_arg->position, remaining);

    if (!unstdubytes_set_length(&context_arg->buffer, remaining, NULL)) {
        unstdcommon_returnM(5);
    }

    context_arg->position = remaining;

    unstdcommon_returnM(1);
}

void unstdbytebuffer_skip(unstdbytebuffer *context_arg, sizet length_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3);
    }


    if (context_arg->limit && context_arg->position + length_arg > context_arg->limit) {
        unstdcommon_returnM(4);
    }

    if (context_arg->position + length_arg > context_arg->buffer.capacity) {
        unstdcommon_returnM(5);
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, length_arg, NULL)) {
        unstdcommon_returnM(6);
    }

    context_arg->position += length_arg;

    unstdcommon_returnM(1);
}

void unstdbytebuffer_reuse(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2);
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3);
    }

    if (!unstdubytes_set_length(&context_arg->buffer, 0, NULL)) {
        unstdcommon_returnM(4);
    }

    context_arg->position = 0;

    unstdcommon_returnM(1);
}

bool unstdbytebuffer_write_uint32(unstdbytebuffer *context_arg, u32t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(u32t) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

    if (context_arg->position + sizeof(u32t) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(u32t)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, sizeof(u32t), NULL)) {
        unstdcommon_returnM(6) false;
    }

    for (sizet i = 0; i < sizeof(u32t); ++i) {
        context_arg->buffer.data[context_arg->position++] = (u8t) ((buffer_arg >> (i * 8)) & 0xFF);
    }

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_int32(unstdbytebuffer *context_arg, s32t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(s32t) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

    if (context_arg->position + sizeof(s32t) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(s32t)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, sizeof(s32t), NULL)) {
        unstdcommon_returnM(6) false;
    }

    for (sizet i = 0; i < sizeof(s32t); ++i) {
        context_arg->buffer.data[context_arg->position++] = (u8t) ((buffer_arg >> (i * 8)) & 0xFF);
    }

    unstdcommon_returnM(1) true;
}


bool unstdbytebuffer_write_uint64(unstdbytebuffer *context_arg, u64t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(u64t) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

    if (context_arg->position + sizeof(u64t) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(u64t)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, sizeof(u64t), NULL)) {
        unstdcommon_returnM(6) false;
    }

    for (sizet i = 0; i < sizeof(u64t); ++i) {
        context_arg->buffer.data[context_arg->position++] = (u8t) ((buffer_arg >> (i * 8)) & 0xFF);
    }

    unstdcommon_returnM(1) true;
}


bool unstdbytebuffer_write_int64(unstdbytebuffer *context_arg, s64t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(s64t) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

    if (context_arg->position + sizeof(s64t) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(s64t)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, sizeof(s64t), NULL)) {
        unstdcommon_returnM(6) false;
    }

    for (sizet i = 0; i < sizeof(s64t); ++i) {
        context_arg->buffer.data[context_arg->position++] = (u8t) ((buffer_arg >> (i * 8)) & 0xFF);
    }

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_byte(unstdbytebuffer *context_arg, u8t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(u8t) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

//    if (context_arg->buffer.length + sizeof(u8t) > context_arg->buffer.capacity)
    if (context_arg->position + sizeof(u8t) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(u8t)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, 1, NULL)) {
        unstdcommon_returnM(6) false;
    }

    context_arg->buffer.data[context_arg->position++] = buffer_arg;

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_vuint(unstdbytebuffer *context_arg, u64t buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    while (buffer_arg >= 0x80) {
        if (!unstdbytebuffer_write_byte(context_arg, (u8t) ((buffer_arg & 0x7F) | 0x80), NULL)) {
            unstdcommon_returnM(3) false;
        }
        buffer_arg >>= 7;
    }

    if (!unstdbytebuffer_write_byte(context_arg, (u8t) (buffer_arg & 0x7F), NULL)) {
        unstdcommon_returnM(3) false;
    }

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_bool(unstdbytebuffer *context_arg, bool buffer_arg, u8t *const out_error_arg) {
    return unstdbytebuffer_write_uint32(context_arg, buffer_arg ? 0xBADBABE5 : 0xBADBEEF5, out_error_arg);
}

bool unstdbytebuffer_write_double(unstdbytebuffer *context_arg, double buffer_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (context_arg->limit && context_arg->position + sizeof(double) > context_arg->limit) {
        unstdcommon_returnM(4) false;
    }

    if (context_arg->position + sizeof(double) > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + sizeof(double)) != 1) {
            unstdcommon_returnM(5) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, 8, NULL)) {
        unstdcommon_returnM(6) false;
    }

    memcpy(context_arg->buffer.data + context_arg->position, &buffer_arg, 8);
    context_arg->position += 8;

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_bytes(unstdbytebuffer *context_arg,
                                 const u8t *buffer_arg, sizet length_arg,
                                 u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (!length_arg) {
        unstdcommon_returnM(4) false;
    }

    if (!buffer_arg) {
        unstdcommon_returnM(5) false;
    }

    if (context_arg->limit && context_arg->position + length_arg > context_arg->limit) {
        unstdcommon_returnM(6) false;
    }

    if (context_arg->position + length_arg > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + length_arg) != 1) {
            unstdcommon_returnM(7) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, length_arg, NULL)) {
        unstdcommon_returnM(8) true;
    }


    unstdbytebuffer_write_bytes_internal(context_arg, buffer_arg, 0, length_arg);

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_writeubytes(unstdbytebuffer *const context_arg,
                                const ubytes *const source_buffer_arg,
                                u8t *const out_error_arg) {
    return unstdbytebuffer_write_bytes(context_arg, source_buffer_arg->data, source_buffer_arg->length, out_error_arg);
}

bool unstdbytebuffer_write_bytes_offset(unstdbytebuffer *context_arg,
                                        const u8t *buffer_arg, sizet offset_arg, sizet length_arg,
                                        u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    if (!length_arg) {
        unstdcommon_returnM(4) false;
    }

    if (!buffer_arg) {
        unstdcommon_returnM(5) false;
    }

    if (context_arg->limit && context_arg->position + length_arg > context_arg->limit) {
        unstdcommon_returnM(6) false;
    }

    if (context_arg->position + length_arg > context_arg->buffer.capacity) {
        if (unstdubytes_grow(&context_arg->buffer, context_arg->buffer.capacity + length_arg) != 1) {
            unstdcommon_returnM(7) false;
        }
    }

    if (!unstdubytes_increase_length(&context_arg->buffer, length_arg, NULL)) {
        unstdcommon_returnM(8) false;
    }

    unstdbytebuffer_write_bytes_internal(context_arg, buffer_arg, offset_arg, length_arg);

    unstdcommon_returnM(1) true;
}

bool unstdbytebuffer_write_bytearray(unstdbytebuffer *context_arg,
                                     const u8t *buffer_arg,
                                     u64t offset_arg,
                                     u64t length_arg,
                                     u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) false;
    }

    if (context_arg->buffer.capacity && !context_arg->buffer.data) {
        unstdcommon_returnM(3) false;
    }

    u8t err;
    if (!unstdbytebuffer_write_vuint(context_arg, length_arg, &err)) {
        unstdcommon_returnM(10 * 1 + err) false;
    }

    if (!unstdbytebuffer_write_bytes(context_arg, buffer_arg + offset_arg, length_arg, &err)) {
        unstdcommon_returnM(10 * 2 + err) false;
    }

    unstdcommon_returnM(1) true;
}


u32t unstdbytebuffer_read_uint32(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0;
    }

    if (context_arg->limit && context_arg->position + sizeof(u32t) > context_arg->limit) {
        unstdcommon_returnM(4) 0;
    }

    if (context_arg->position + sizeof(u32t) > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) 0;
    }

    if (context_arg->position + sizeof(u32t) > context_arg->buffer.length) {
        unstdcommon_returnM(6) 0;
    }

    u32t v = 0;
    for (sizet i = 0; i < sizeof(u32t); ++i) {
        v |= ((u32t) context_arg->buffer.data[context_arg->position++]) << (i * 8);
    }

    unstdcommon_returnM(1) v;
}

s32t unstdbytebuffer_read_int32(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    return (s32t) unstdbytebuffer_read_uint32(context_arg, out_error_arg);
}

u64t unstdbytebuffer_read_uint64(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0;
    }

    if (context_arg->limit && context_arg->position + sizeof(u64t) > context_arg->limit) {
        unstdcommon_returnM(4) 0;
    }

    if (context_arg->position + sizeof(u64t) > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) 0;
    }

    if (context_arg->position + sizeof(u64t) > context_arg->buffer.length) {
        unstdcommon_returnM(6) 0;
    }

    u64t v = 0;
    for (sizet i = 0; i < sizeof(u64t); ++i) {
        v |= ((u64t) context_arg->buffer.data[context_arg->position++]) << (i * 8);
    }

    unstdcommon_returnM(1) v;
}

s64t unstdbytebuffer_read_int64(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    return (s64t) unstdbytebuffer_read_uint64(context_arg, out_error_arg);
}

bool unstdbytebuffer_read_bool(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    return unstdbytebuffer_read_uint32(context_arg, out_error_arg) == 0xBADBABE5;
}

double unstdbytebuffer_read_double(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0.0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0.0;
    }

    if (context_arg->limit && context_arg->position + 8 > context_arg->limit) {
        unstdcommon_returnM(4) 0.0;
    }

    if (context_arg->position + 8 > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) 0.0;
    }

    if (context_arg->position + 8 > context_arg->buffer.length) {
        unstdcommon_returnM(6) 0.0;
    }

    double value = 0;
    memcpy(&value, context_arg->buffer.data + context_arg->position, 8);
    context_arg->position += 8;

    unstdcommon_returnM(1) value;
}

u8t unstdbytebuffer_read_byte(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) 0;
    }

    if (context_arg->limit && context_arg->position + sizeof(u8t) > context_arg->limit) {
        unstdcommon_returnM(4) 0;
    }

    if (context_arg->position + sizeof(u8t) > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) 0;
    }

    if (context_arg->position + sizeof(u8t) > context_arg->buffer.length) {
        unstdcommon_returnM(6) 0;
    }

    unstdcommon_returnM(1) context_arg->buffer.data[context_arg->position++];
}

u64t unstdbytebuffer_read_vuint(unstdbytebuffer *context_arg, u8t maxv_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) 0;
    }

    u64t result = 0;
    u8t shift = 0;
    u8t error_out = 0;
    u8t byte;

    u8t maxv = maxv_arg ? maxv_arg : 4;
    for (int i = 0; i < maxv; i++) {
        byte = unstdbytebuffer_read_byte(context_arg, &error_out);

        if (error_out != 1) {
            unstdcommon_returnM(3) 0;
        }

        result |= ((u64t) (byte & 0x7F)) << shift;
        if ((byte & 0x80) == 0) {
            unstdcommon_returnM(1) result;
        }
        shift += 7;
    }

    unstdcommon_returnM(4) result; /* malformed varuint (too long) */
}

u8t *unstdbytebuffer_read_bytes(unstdbytebuffer *context_arg, sizet length_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) NULL;
    }

    if (context_arg->position + length_arg > context_arg->limit) {
        unstdcommon_returnM(4) NULL;
    }

    if (context_arg->position + length_arg > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) NULL;
    }

    if (context_arg->position + length_arg > context_arg->buffer.length) {
        unstdcommon_returnM(6) NULL;
    }

    u8t *result = context_arg->buffer.data + context_arg->position;
    context_arg->position += length_arg;

    unstdcommon_returnM(1) result;
}

u8t *unstdbytebuffer_read_bytes_copy(unstdbytebuffer *context_arg, sizet length_arg, u8t *const out_error_arg) {
    if (!context_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) NULL;
    }

    if (context_arg->position + length_arg > context_arg->limit) {
        unstdcommon_returnM(4) NULL;
    }

    if (context_arg->position + length_arg > context_arg->buffer.capacity) {
        unstdcommon_returnM(5) NULL;
    }

    if (context_arg->position + length_arg > context_arg->buffer.length) {
        unstdcommon_returnM(6) NULL;
    }

    u8t *copy_data = malloc(length_arg);
    if (!copy_data) {
        unstdcommon_returnM(7) NULL;
    }

    memcpy(copy_data, context_arg->buffer.data + context_arg->position, length_arg);
    context_arg->position += length_arg;

    unstdcommon_returnM(1) copy_data;
}


ubytes unstdbytebuffer_read_bytearray(unstdbytebuffer *context_arg,
                                      u8t *const out_error_arg) {
    ubytes result = {0};

    if (!context_arg) {
        unstdcommon_returnM(2) result;
    }

    u8t error_result = 0;
    u64t length = unstdbytebuffer_read_vuint(context_arg, 5, &error_result);
    if (length == 0 || error_result != 1) {
        unstdcommon_returnM(3) result;
    }

    error_result = 0;
    u8t *slice = unstdbytebuffer_read_bytes(context_arg, length, &error_result);
    if (!slice || error_result != 1) {
        unstdcommon_returnM(4) result;
    }

    result.data = slice;
    result.length = (sizet) length;
    result.capacity = (sizet) length;

    unstdcommon_returnM(1) result;
}

ubytes unstdbytebuffer_read_bytearray_copy(unstdbytebuffer *context_arg, u8t *const out_error_arg) {
    ubytes result = {0};

    if (!context_arg) {
        unstdcommon_returnM(2) result;
    }

    if (!context_arg->buffer.data) {
        unstdcommon_returnM(3) result;
    }

    u8t error_result = 0;
    u64t length = unstdbytebuffer_read_vuint(context_arg, 5, &error_result);
    if (length == 0 || error_result != 1) {
        unstdcommon_returnM(4) result;
    }

    error_result = 0;
    u8t *data_copy_ptr = unstdbytebuffer_read_bytes_copy(context_arg, (sizet) length, &error_result);
    if (error_result != 1) {
        unstdcommon_returnM(5) result;
    }

    result.data = data_copy_ptr;
    result.length = length;
    result.capacity = length;

    unstdcommon_returnM(1) result;
}
