#ifndef UNSTDLIB_UNSTDSTRING_COMPAT_H
#define UNSTDLIB_UNSTDSTRING_COMPAT_H

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "_unstdcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

//! [strlen]
/**
 * @brief Counts the number of UTF-8 characters (bytes (i8t)) inside `buffer_arg`.
 * @note Returns 0 if `buffer_arg` is NULL.
 * @param buffer_arg Should be a pointer to a valid, null-terminated buffer.
 * @returns Number of UTF-8 characters (bytes (i8t)) in `buffer_arg`.
 */
extern sizet unstdstring_compat_strlen8(
        const i8 *const buffer_arg
);

/**
 * @brief Counts the number of UTF-8 characters (bytes (u8t)) inside `buffer_arg`.
 * @note Returns 0 if `buffer_arg` is NULL.
 * @param buffer_arg Should be a pointer to a valid, null-terminated buffer.
 * @returns Number of UTF-8 characters (bytes (u8t)) in `buffer_arg`.
 */
extern sizet unstdstring_compat_strlenu8(
        const u8t *const buffer_arg
);

/**
 * @brief Counts the number of UTF-16 characters (double-bytes (u16t)) inside `buffer_arg`.
 * @note Returns 0 if `buffer_arg` is NULL.
 * @param buffer_arg Should be a pointer to a valid, null-terminated buffer.
 * @returns Number of UTF-16 characters (double-bytes (u16t)) in `buffer_arg`.
 */
extern sizet unstdstring_compat_strlenu16(
        const u16t *const buffer_arg
);


//! [compare]
/**
 *
 * @param f_buffer_arg Should be an UTF-8 character.
 * @param s_buffer_arg Should be a UTF-8 character.
 * @returns True if both buffers are identical otherwise False.
 */
extern bool unstdstring_compat_charcmp8(
        const i8 f_buffer_arg,
        const i8 s_buffer_arg
);

/**
 *
 * @param f_buffer_arg Should be an UTF-32 character.
 * @param s_buffer_arg Should be a UTF-32 character.
 * @returns True if both buffers are identical otherwise False.
 */
extern bool unstdstring_compat_charcmp32(
        const i32 f_buffer_arg,
        const i32 s_buffer_arg
);

/**
 *
 * @param f_buffer_arg Should be an UTF-8 character.
 * @param s_buffer_arg Should be a UTF-8 character.
 * @returns True if both buffers are identical otherwise False.
 */
extern bool unstdstring_compat_charcmpu8(
        const u8t f_buffer_arg,
        const u8t s_buffer_arg
);

/**
 *
 * @param f_buffer_arg Should be an UTF-16 character.
 * @param s_buffer_arg Should be a UTF-16 character.
 * @returns True if both buffers are identical otherwise False.
 */
extern bool unstdstring_compat_charcmpu16(
        const u16t f_buffer_arg,
        const u16t s_buffer_arg
);

/**
 *
 * @param f_buffer_arg Should be an UTF-32 character.
 * @param s_buffer_arg Should be a UTF-32 character.
 * @returns True if both buffers are identical otherwise False.
 */
extern bool unstdstring_compat_charcmpu32(
        const u32t f_buffer_arg,
        const u32t s_buffer_arg
);


/**
 * @brief Compares two null-terminated ASCII strings for equality.
 *
 * @param f_buffer_arg First string buffer (`const i8*`).
 * @param s_buffer_arg Second string buffer (`const i8*`).
 *
 * @returns `true` if both strings are non-null, null-terminated, and contain identical characters.
 *          `false` otherwise.
 *
 * @note Returns `true` if both `*f_buffer_arg` and `*s_buffer_arg` are `0` (i.e., both strings are empty).
 */
bool unstdstring_compat_strequal8(
        const i8 *const f_buffer_arg,
        const i8 *const s_buffer_arg
);

/**
 * @brief Compares two null-terminated ASCII strings for equality, ignoring case.
 *
 * @param f_buffer_arg First string buffer (`const i8*`).
 * @param s_buffer_arg Second string buffer (`const i8*`).
 *
 * @returns `true` if both strings are non-null, null-terminated, and contain identical characters
 *          when compared case-insensitively. `false` otherwise.
 *
 * @note Returns `true` if both `*f_buffer_arg` and `*s_buffer_arg` are `0` (i.e., both strings are empty).
 */
bool unstdstring_compat_strequali8(
        const i8 *const f_buffer_arg,
        const i8 *const s_buffer_arg
);

// todo: doc this
i8 *unstdstring_compat_strchr(
        const i8 *buffer_arg,
        i32 char_arg
);

/**
 * @brief Finds the first occurrence of a substring in a null-terminated ASCII string.
 *
 * @param buffer_arg The source string (`const i8*`) to search within.
 * @param substring_arg The substring (`const i8*`) to find.
 *
 * @returns A pointer (`i8*`) to the first occurrence of `substring_arg` within `buffer_arg`,
 *          or `NULL` if the substring is not found.
 *
 * @note If `substring_arg` is empty (`*substring_arg == 0`), the function returns `buffer_arg`.
 */
i8 *unstdstring_compat_strstr(
        const i8 *buffer_arg,
        const i8 *const substring_arg
);

/**
 * @brief Finds the first occurrence of a substring in a null-terminated ASCII string, ignoring case.
 *
 * @param buffer_arg The source string (`const i8*`) to search within.
 * @param substring_arg The substring (`const i8*`) to find.
 *
 * @returns A pointer (`i8*`) to the first occurrence of `substring_arg` within `buffer_arg`,
 *          ignoring character case, or `NULL` if the substring is not found.
 *
 * @note If `substring_arg` is empty (`*substring_arg == 0`), the function returns `buffer_arg`.
 */
i8 *unstdstring_compat_strcasestr(
        const i8 *buffer_arg,
        const i8 *const substring_arg
);


/**
 * @brief Concatenates two null-terminated ASCII strings into a newly allocated buffer.
 *
 * @param f_buffer_arg First string (`const i8*`).
 * @param s_buffer_arg Second string (`const i8*`).
 *
 * @returns A newly allocated string (`i8*`) containing the concatenation of both strings,
 *          or `NULL` if both inputs are empty or memory allocation fails.
 *
 * @note If one input is empty or `NULL`, the function returns a pointer to the other input.
 *       The returned string must be freed by the caller if it was allocated.
 */
i8 *unstdstring_compat_strcat(
        const i8 *const f_buffer_arg,
        const i8 *const s_buffer_arg
);

/**
 * @brief Compares two null-terminated ASCII strings lexicographically.
 *
 * @param f_buffer_arg First string (`const i8*`).
 * @param s_buffer_arg Second string (`const i8*`).
 *
 * @returns `0` if both strings are equal,
 *          a positive value if the first differing character in `f_buffer_arg` is greater,
 *          or a negative value if it is smaller.
 *          Returns `-1` if either input is `NULL`.
 */
i32 unstdstring_compat_strcmp(
        const i8 *const f_buffer_arg,
        const i8 *const s_buffer_arg
);


/**
 * @brief Compares up to `n_arg` characters of two null-terminated ASCII strings lexicographically.
 *
 * @param f_buffer_arg First string (`const i8*`).
 * @param s_buffer_arg Second string (`const i8*`).
 * @param n_arg Maximum number of characters to compare (`const sizet`).
 *
 * @returns `0` if the compared parts of both strings are equal,
 *          a positive value if the first differing character in `f_buffer_arg` is greater,
 *          or a negative value if it is smaller.
 *          Returns `-1` if either input is `NULL`.
 */
i32 unstdstring_compat_strncmp(
        const i8 *const f_buffer_arg,
        const i8 *const s_buffer_arg,
        const sizet n_arg
);


#ifdef __cplusplus
}
#endif

#endif /* UNSTDLIB_UNSTDSTRING_COMPAT_H */
