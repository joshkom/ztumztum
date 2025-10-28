#include "unstdstring.h"

#include <ctype.h>
#include "unstdinttypes.h"
#include "unstdmemory.h"
#include "unstdbool.h"
#include "_unstdcommon.h"
#include <stdlib.h>
#include <string.h>


typedef enum : u8t {
    _unstdstring_bufferencoding_UTF8 = 1,
    _unstdstring_bufferencoding_UTF16 = 2,
    _unstdstring_bufferencoding_UTF32 = 4
} _unstdstring_bufferencoding;


unstdstring *unstdstring_new(const i8 *const buffer_arg, const sizet max_capacity_arg, u8t *const out_error_arg) {
    unstdstring *string = {0};

    if (buffer_arg && *buffer_arg) {
        u64lt buffer_arg_size = strlen(buffer_arg);
        if (max_capacity_arg && max_capacity_arg < buffer_arg_size) {
            unstdcommon_returnM(3) NULL;
        }

        string = unstdmemory_type_callocM(unstdstring);

        if (!string) {
            unstdcommon_returnM(2) NULL;
        }

        string->data = (i8 *) calloc(
                buffer_arg_size + 1,
                _unstdstring_bufferencoding_UTF8
        );
        if (!string->data) {
            unstdcommon_returnM(4) NULL;
        }

        memcpy(string->data, buffer_arg, buffer_arg_size);

        string->limit = max_capacity_arg;
        string->length = buffer_arg_size;
        string->capacity = buffer_arg_size + _unstdstring_bufferencoding_UTF8;
    } else {
        string = unstdmemory_type_callocM(unstdstring);

        if (!string) {
            unstdcommon_returnM(2) NULL;
        }

        string->data = (i8 *) calloc(
                1 + 1,
                _unstdstring_bufferencoding_UTF8
        );

        if (!string->data) {
            unstdcommon_returnM(4) NULL;
        }

        string->limit = max_capacity_arg;
        string->length = 0;
        string->capacity = 1 + _unstdstring_bufferencoding_UTF8;
    }

    unstdcommon_returnM(1) string;
}

u8t unstdstring_set(unstdstring *buffer_arg, const i8 *new_string) {
    if (!buffer_arg) { return 2; }

    if (!new_string) { return 3; }

    sizet new_str_len = strlen(new_string);
    sizet required_size = new_str_len * _unstdstring_bufferencoding_UTF8 + _unstdstring_bufferencoding_UTF8;

    if (buffer_arg->limit && required_size > buffer_arg->limit) {
        return 4;
    }

    if (required_size > buffer_arg->capacity) {
        i8 *new_data = (i8 *) realloc(buffer_arg->data, required_size);
        if (!new_data) { return 5; }

        buffer_arg->data = new_data;
        buffer_arg->capacity = required_size;
    }

    memcpy(buffer_arg->data, new_string, new_str_len);
    buffer_arg->data[new_str_len] = 0x0;
    buffer_arg->length = new_str_len;

    return 1;
}

u8t unstdstring_free(unstdstring *buffer_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (!buffer_arg->capacity) {
        return 3;
    }

    free(buffer_arg->data);
    free(buffer_arg);

    return 1;
}

u8t unstdstring_reset(unstdstring *buffer_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (!buffer_arg->capacity) {
        return 3;
    }

    memset(buffer_arg->data, 0, buffer_arg->capacity);

    i8 *temp_buffer_0 = realloc(buffer_arg->data, _unstdstring_bufferencoding_UTF8 + 1);
    if (!temp_buffer_0) {
        return 4;
    }

    buffer_arg->data = temp_buffer_0;
    buffer_arg->capacity = _unstdstring_bufferencoding_UTF8 + 1;
    buffer_arg->length = 0;

    return 1;
}

u8t unstdstring_clear(unstdstring *buffer_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (!buffer_arg->capacity) {
        return 3;
    }

    memset(buffer_arg->data, 0, buffer_arg->capacity);

    buffer_arg->length = 0;

    return 1;
}


bool unstdstring_equal(const unstdstring *const f_buffer_arg,
                       const unstdstring *const s_buffer_arg,
                       u8t *const out_error_arg) {
    if (!f_buffer_arg || !s_buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!f_buffer_arg->data || !s_buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*f_buffer_arg->data && !*s_buffer_arg->data) {
        unstdcommon_returnM(1) true;
    } else if (!*f_buffer_arg->data || !*s_buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (f_buffer_arg->length != s_buffer_arg->length) {
        unstdcommon_returnM(1) false;
    }

    unstdcommon_returnM(1) !strcmp(f_buffer_arg->data, s_buffer_arg->data);
}

bool unstdstring_equal_overload_0(const unstdstring *const f_buffer_arg,
                                  const i8 *const s_buffer_arg,
                                  u8t *const out_error_arg) {
    if (!f_buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!f_buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!s_buffer_arg) {
        unstdcommon_returnM(4) false;
    }

    if (!*f_buffer_arg->data && !*s_buffer_arg) {
        unstdcommon_returnM(1) true;
    } else if (!*f_buffer_arg->data || !*s_buffer_arg) {
        unstdcommon_returnM(5) false;
    }

    const sizet s_length = strlen((const i8 *) s_buffer_arg);

    if (f_buffer_arg->length != s_length) {
        unstdcommon_returnM(1) false;
    }

    unstdcommon_returnM(1) !strcmp(f_buffer_arg->data, s_buffer_arg);
}

bool unstdstring_equal_ignorecase(const unstdstring *const f_buffer_arg,
                                  const unstdstring *const s_buffer_arg,
                                  u8t *const out_error_arg) {
    if (!f_buffer_arg || !s_buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!f_buffer_arg->data || !s_buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*f_buffer_arg->data && !*s_buffer_arg->data) {
        unstdcommon_returnM(1) true;
    } else if (!*f_buffer_arg->data || !*s_buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (f_buffer_arg->length != s_buffer_arg->length) {
        unstdcommon_returnM(1) false;
    }

    for (sizet buffer_arg_ptr = 0; buffer_arg_ptr < f_buffer_arg->length; buffer_arg_ptr++) {
        if (tolower(((const u8t *) s_buffer_arg->data)[buffer_arg_ptr]) !=
            tolower(((const u8t *) f_buffer_arg->data)[buffer_arg_ptr])) {
            unstdcommon_returnM(1) false;
        }
    }

    unstdcommon_returnM(1) true;
}

bool unstdstring_equal_ignorecase_overload_0(const unstdstring *const f_buffer_arg,
                                             const i8 *const s_buffer_arg,
                                             u8t *const out_error_arg) {
    if (!f_buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!f_buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!s_buffer_arg) {
        unstdcommon_returnM(4) false;
    }

    if (!*f_buffer_arg->data && !*s_buffer_arg) {
        unstdcommon_returnM(1) true;
    } else if (!*f_buffer_arg->data || !*s_buffer_arg) {
        unstdcommon_returnM(5) false;
    }

    const sizet s_length = strlen((const i8 *) s_buffer_arg);

    if (f_buffer_arg->length != s_length) {
        unstdcommon_returnM(1) false;
    }

    for (sizet buffer_arg_ptr = 0; buffer_arg_ptr < f_buffer_arg->length; buffer_arg_ptr++) {
        if (tolower(((const u8t *) f_buffer_arg->data)[buffer_arg_ptr]) !=
            tolower(((const u8t *) s_buffer_arg)[buffer_arg_ptr])) {
            unstdcommon_returnM(1) false;
        }
    }

    unstdcommon_returnM(1) true;
}


bool unstdstring_startswith_char(const unstdstring *const buffer_arg, const i8 checkon_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    unstdcommon_returnM(1) *buffer_arg->data == checkon_arg;
}

bool unstdstring_startswith_char_ignorecase(const unstdstring *const buffer_arg,
                                            const i8 checkon_arg,
                                            u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    unstdcommon_returnM(1) tolower((u8t) *buffer_arg->data) == tolower((u8t) checkon_arg);
}


bool unstdstring_endswith_char(const unstdstring *const buffer_arg, const i8 checkon_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (buffer_arg->length < 1) {
        unstdcommon_returnM(5) false;
    }

    unstdcommon_returnM(1) buffer_arg->data[buffer_arg->length - 1] == checkon_arg;
}


bool unstdstring_endswith_char_ignorecase(const unstdstring *const buffer_arg,
                                          const i8 checkon_arg,
                                          u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (buffer_arg->length < 1) {
        unstdcommon_returnM(5) false;
    }

    unstdcommon_returnM(1) tolower((u8t) buffer_arg->data[buffer_arg->length - 1]) == tolower(checkon_arg);
}

bool unstdstring_startswith_str(const unstdstring *const buffer_arg,
                                const unstdstring *const checkon_arg,
                                u8t *const out_error_arg) {
    if (!buffer_arg || !checkon_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data || !checkon_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data && !*checkon_arg->data) {
        unstdcommon_returnM(1) true;
    } else if (!*buffer_arg->data || !*checkon_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (buffer_arg->length < checkon_arg->length) {
        unstdcommon_returnM(5) false;
    }

    unstdcommon_returnM(1) memcmp(buffer_arg->data, checkon_arg->data, checkon_arg->length) == 0;
}

bool unstdstring_endswith_str(const unstdstring *const buffer_arg,
                              const unstdstring *const checkon_arg,
                              u8t *const out_error_arg) {
    if (!buffer_arg || !checkon_arg) {
        unstdcommon_returnM(2) false;
    }

    if (!buffer_arg->data || !checkon_arg->data) {
        unstdcommon_returnM(3) false;
    }

    if (!*buffer_arg->data && !*checkon_arg->data) {
        unstdcommon_returnM(1) true;
    } else if (!*buffer_arg->data || !*checkon_arg->data) {
        unstdcommon_returnM(4) false;
    }

    if (buffer_arg->length < checkon_arg->length) {
        unstdcommon_returnM(5) false;
    }

    unstdcommon_returnM(1) memcmp(
            buffer_arg->data + buffer_arg->length - checkon_arg->length,
            checkon_arg->data,
            checkon_arg->length
    ) == 0;
}


void unstdstring_tolower_str(const unstdstring *const buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2);
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3);
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4);
    }

    for (sizet i = 0; i < buffer_arg->length; i++) {
        buffer_arg->data[i] = (i8) tolower((u8t) buffer_arg->data[i]);
    }

    unstdcommon_returnM(1);
}

unstdstring *unstdstring_tolower_str_copy(const unstdstring *const buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) NULL;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) NULL;
    }

    i8 *l_copy_ptr = (i8 *) calloc(buffer_arg->length + 1, sizeof(i8));
    if (!l_copy_ptr) {
        unstdcommon_returnM(5) NULL;
    }

    for (sizet i = 0; i < buffer_arg->length; i++) {
        l_copy_ptr[i] = (i8) tolower((u8t) buffer_arg->data[i]);
    }

    u8t err = 0;
    unstdstring *string = unstdstring_new(l_copy_ptr, buffer_arg->limit, &err);

    free(l_copy_ptr);

    if (err != 1) {
        unstdcommon_returnM(6) NULL;
    }

    unstdcommon_returnM(1) string;
}

void unstdstring_toupper_str(const unstdstring *const buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2);
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3);
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4);
    }

    for (sizet i = 0; i < buffer_arg->length; i++) {
        buffer_arg->data[i] = (i8) toupper((u8t) buffer_arg->data[i]);
    }

    unstdcommon_returnM(1);
}

unstdstring *unstdstring_toupper_str_copy(const unstdstring *const buffer_arg, u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) NULL;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) NULL;
    }

    i8 *l_copy_ptr = (i8 *) calloc(buffer_arg->length + 1, sizeof(i8));
    if (!l_copy_ptr) {
        unstdcommon_returnM(5) NULL;
    }

    for (sizet i = 0; i < buffer_arg->length; i++) {
        l_copy_ptr[i] = (i8) toupper((u8t) buffer_arg->data[i]);
    }

    u8t err = 0;
    unstdstring *string = unstdstring_new(l_copy_ptr, buffer_arg->limit, &err);

    free(l_copy_ptr);

    if (err != 1) {
        unstdcommon_returnM(6) NULL;
    }

    unstdcommon_returnM(1) string;
}


bool unstdstring_isasciicontrol_char(const u8t buffer_arg) {
    return (buffer_arg >= 0 && buffer_arg <= 31) || buffer_arg == 127;
}


bool unstdstring_isasciiprintable_char(const u8t buffer_arg) {
    return buffer_arg >= 32 && buffer_arg <= 126;
}


bool unstdstring_isasciiextended_char(const u8t buffer_arg) {
    return buffer_arg >= 128 && buffer_arg <= 255;
}

bool unstdstring_isasciivisible_char(const u8t buffer_arg) {
    return unstdstring_isasciiprintable_char(buffer_arg)
           || unstdstring_isasciiextended_char(buffer_arg);
}

bool unstdstring_isascii_char(const u8t buffer_arg) {
    return buffer_arg >= 0 && buffer_arg <= 255;
}

bool unstdstring_isalphabetic_char(const u8t buffer_arg) {
    return (buffer_arg >= 'a' && buffer_arg <= 'z')
           || (buffer_arg >= 'A' && buffer_arg <= 'Z');
}


bool unstdstring_isalphanumeric_char(const u8t buffer_arg) {
    return unstdstring_isalphabetic_char(buffer_arg)
           || (buffer_arg >= '0' && buffer_arg <= '9');
}


bool unstdstring_isdigit_char(const u8t buffer_arg) {
    return buffer_arg >= '0' && buffer_arg <= '9';
}


bool unstdstring_ishex_char(const u8t buffer_arg) {
    return unstdstring_isdigit_char(buffer_arg)
           || (buffer_arg >= 'a' && buffer_arg <= 'f')
           || (buffer_arg >= 'A' && buffer_arg <= 'F');
}


bool unstdstring_iswhitespace_char(const u8t buffer_arg) {
    return buffer_arg == 32 || buffer_arg == 9
           || buffer_arg == 10 || buffer_arg == 11
           || buffer_arg == 12 || buffer_arg == 13;
}


u8t _unstdstring_grow(unstdstring *const buffer_arg, const sizet bytes_arg, const u8t encoding_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (bytes_arg < 1) {
        return 3;
    }

    u64lt size_buffer_arg;
    switch (encoding_arg) {
        case _unstdstring_bufferencoding_UTF8:
            size_buffer_arg = !*(i8 *) buffer_arg->data ? 0 : buffer_arg->length;
            break;
        case _unstdstring_bufferencoding_UTF16:
            //! Not implemented yet
//            size_buffer_arg = !*(u16t *) buffer_arg ? 0 : unstdstring_strlen16(buffer_arg);
//            break;
        case _unstdstring_bufferencoding_UTF32:
            //! Not implemented yet
//            size_buffer_arg = !*(u32t *) buffer_arg ? 0 : unstdstring_strlen32(buffer_arg);
//            break;
        default:
            return 4;
    }

//    const sizet realloc_size = size_buffer_arg * encoding_arg + bytes_arg * encoding_arg + encoding_arg;
    const sizet realloc_size = buffer_arg->capacity + bytes_arg * encoding_arg;

    if (buffer_arg->limit && realloc_size > buffer_arg->limit) {
        return 5;
    }

    i8 *realloc_result = (i8 *) realloc(
            (void *) buffer_arg->data,
            realloc_size
    );

    if (!realloc_result) {
        return 6;
    }

    buffer_arg->data = realloc_result;
    buffer_arg->capacity = realloc_size;

    if (!memset((i8 *) buffer_arg->data + size_buffer_arg * encoding_arg,
                0,
                bytes_arg * encoding_arg + encoding_arg)) {
        return 7;
    }

    return 1;
}


u8t _unstdstring_shrink(unstdstring *const buffer_arg, const sizet bytes_arg, const u8t encoding_arg) {
    if (!buffer_arg) {
        return 2;
    }

    if (bytes_arg < 1) {
        return 3;
    }

    sizet size_buffer_arg;
    switch (encoding_arg) {
        case _unstdstring_bufferencoding_UTF8:
            size_buffer_arg = !*(i8 *) buffer_arg->data ? 0 : buffer_arg->length;
            break;
        case _unstdstring_bufferencoding_UTF16:
            //! Not implemented yet
//            size_buffer_arg = !*(u16t *) buffer_arg ? 0 : unstdstring_strlen16(buffer_arg);
//            break;
        case _unstdstring_bufferencoding_UTF32:
            //! Not implemented yet
//            size_buffer_arg = !*(u32t *) buffer_arg ? 0 : unstdstring_strlen32(buffer_arg);
//            break;
        default:
            return 6;
    }

    if (size_buffer_arg < 1) {
        return 4;
    }

    if (bytes_arg > buffer_arg->capacity) {
        return 5;
    }

    const sizet realloc_size = buffer_arg->capacity - bytes_arg * encoding_arg;

    if (!memset(buffer_arg->data + size_buffer_arg * encoding_arg - bytes_arg * encoding_arg,
                0,
                bytes_arg * encoding_arg + encoding_arg)) {
        return 7;
    }

    i8 *realloc_result = (i8 *) realloc(
            buffer_arg->data,
            realloc_size
    );

    if (!realloc_result) {
        return 8;
    }

    buffer_arg->data = realloc_result;
    buffer_arg->capacity = realloc_size;

    return 1;
}


u8t unstdstring_pushback_char(unstdstring *const to_buffer_arg, const i8 from_buffer_arg) {
    if (!to_buffer_arg) {
        return 2;
    }

    if (!to_buffer_arg->data) {
        return 3;
    }

    if (to_buffer_arg->length
        + 1 * _unstdstring_bufferencoding_UTF8
        + 1 * _unstdstring_bufferencoding_UTF8 > to_buffer_arg->capacity) {
        if (_unstdstring_grow(to_buffer_arg,
                              1 * _unstdstring_bufferencoding_UTF8,
                              _unstdstring_bufferencoding_UTF8) != 1) {
            return 4;
        }
    }

    to_buffer_arg->data[to_buffer_arg->length++] = from_buffer_arg;
    to_buffer_arg->data[to_buffer_arg->length] = 0;

    return 1;
}


u8t unstdstring_pushback_str(unstdstring *restrict const to_buffer_arg, const i8 *const from_buffer_arg) {
    if (!to_buffer_arg) {
        return 2;
    }

    if (!to_buffer_arg->data) {
        return 3;
    }

    if (!from_buffer_arg) {
        return 4;
    }

    if (!*from_buffer_arg) {
        return 5;
    }

    const sizet length_from_buffer_arg_temp = strlen(from_buffer_arg);

    if (to_buffer_arg->length
        + length_from_buffer_arg_temp * _unstdstring_bufferencoding_UTF8
        + 1 * _unstdstring_bufferencoding_UTF8 > to_buffer_arg->capacity) {
        if (_unstdstring_grow(to_buffer_arg,
                              length_from_buffer_arg_temp * _unstdstring_bufferencoding_UTF8,
                              _unstdstring_bufferencoding_UTF8) != 1) {
            return 6;
        }
    }

    to_buffer_arg->length += length_from_buffer_arg_temp;
    strcat(to_buffer_arg->data, from_buffer_arg);

    return 1;
}


i8 unstdstring_popback_char(unstdstring *restrict const to_buffer_arg, u8t *const out_error_arg) {
    if (!to_buffer_arg) {
        unstdcommon_returnM(2) 0;
    }

    if (!to_buffer_arg->data) {
        unstdcommon_returnM(3) 0;
    }

    if (!*to_buffer_arg->data) {
        unstdcommon_returnM(4) 0;
    }

    i8 temp_char_holder = to_buffer_arg->data[to_buffer_arg->length - _unstdstring_bufferencoding_UTF8];

    if (_unstdstring_shrink(to_buffer_arg, 1, _unstdstring_bufferencoding_UTF8) != 1) {
        unstdcommon_returnM(5) 0;
    }

    to_buffer_arg->length -= 1;

    unstdcommon_returnM(1) temp_char_holder;
}

unstdstring *unstdstring_substr_copy(unstdstring *const buffer_arg,
                                     const sizet start_arg,
                                     const sizet span_arg,
                                     u8t *const out_error_arg) {
    if (!buffer_arg) {
        unstdcommon_returnM(2) NULL;
    }

    if (!buffer_arg->data) {
        unstdcommon_returnM(3) NULL;
    }

    if (!*buffer_arg->data) {
        unstdcommon_returnM(4) NULL;
    }

    u64lt buffer_arg_len = buffer_arg->length;

    if (buffer_arg_len <= start_arg) {
        unstdcommon_returnM(5) NULL;
    }

    u64lt substr_len = (span_arg <= 0 || span_arg >= buffer_arg_len - start_arg
                        ? buffer_arg_len - start_arg
                        : span_arg) * _unstdstring_bufferencoding_UTF8;

    const sizet calloc_size = substr_len + 1 * _unstdstring_bufferencoding_UTF8;

    i8 *buffer = calloc(substr_len + 1, _unstdstring_bufferencoding_UTF8);

    if (!buffer) {
        unstdcommon_returnM(6) NULL;
    }

    memcpy(buffer, &buffer_arg->data[start_arg], substr_len);

    u8t err = 0;
    unstdstring *string = unstdstring_new(NULL, 0, &err);

    if (err != 1) {
        unstdcommon_returnM(7) NULL;
    }

    free(string->data);
    string->data = buffer;
    string->capacity = calloc_size;

    unstdcommon_returnM(1) string;
}
