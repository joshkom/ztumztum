#ifndef UNSTDLIB_UNSTDSTRING_H
#define UNSTDLIB_UNSTDSTRING_H

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "_unstdcommon.h"


/**
 * @brief Macros for compile-time string encoding in various formats.
 */
#define _unstdstring_encode_as_utf8M(string_arg) u8##string_arg
#define _unstdstring_encode_as_utf16M(string_arg) u##string_arg
#define _unstdstring_encode_as_utf32M(string_arg) U##string_arg

#define _unstdstring_encode_as_widecharM(string_arg) L##string_arg
#define _unstdstring_encode_as_rawM(string_arg) R##string_arg
#define _unstdstring_encode_as_raw_widecharM(string_arg) LR##string_arg

#define _unstdstring_encode_as_raw_utf8M(string_arg) u8R##string_arg
#define _unstdstring_encode_as_raw_utf16M(string_arg) uR##string_arg
#define _unstdstring_encode_as_raw_utf32M(string_arg) UR##string_arg


typedef struct {
    i8 *data;
    sizet length;
    sizet capacity;
    sizet limit;
} unstdstring;


//! [Constructor]
/**
 * @brief Creates a new `unstdstring` object.
 *
 * @param buffer_arg (Optional) A pointer to a null-terminated string used to initialize the `unstdstring` object.
 *                   If `NULL` or an empty string, the object is initialized with an empty buffer.
 * @param max_capacity_arg (Required if `buffer_arg` is not NULL) The maximum capacity of the `unstdstring`.
 *                         Must be greater than or equal to the length of `buffer_arg` if provided. If `buffer_arg`
 *                         is NULL or empty, any value is accepted.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 *
 * @returns A pointer to a newly created `unstdstring` object, or `NULL` if the creation fails.
 *
 * @OutParam <strong>out_error_arg</strong> A status code (`u8t`) indicating the result of the function:
 * @OutParamValue [1] Success. The string was created successfully.
 * @OutParamValue [2] Failure. Memory allocation for the `unstdstring` object failed.
 * @OutParamValue [3] Failure. `max_capacity_arg` is smaller than the size of `buffer_arg`.
 * @OutParamValue [4] Failure. Memory allocation for the internal data buffer failed.
 *
 * @note If `buffer_arg` is provided and non-empty, its content is copied into the newly allocated `unstdstring` buffer.
 *       The function ensures that the buffer is UTF-8 aligned as defined by `_unstdstring_bufferencoding_UTF8`.
 *       The resulting structure includes metadata for maximum capacity, current length, and allocated buffer size.
 */
extern unstdstring *unstdstring_new(
        const i8 *const buffer_arg,
        const sizet max_capacity_arg,
        u8t *const out_error_arg
);

/**
 * @brief Sets the contents of the string buffer to a new null-terminated string.
 *
 * @details This function replaces the contents of `buffer_arg` with a copy of `new_string`.
 *          It calculates the required space based on the length of the input string and
 *          the encoding factor. If the current buffer is too small, it reallocates memory.
 *          The function respects the `limit` field, if set, and will fail if the required
 *          space exceeds this limit. The copied string is null-terminated. The string
 *          buffer's length and capacity fields are updated accordingly.
 *
 * @param buffer_arg     Should be a pointer to a valid `unstdstring` structure.
 * @param new_string        Should be a pointer to a valid null-terminated UTF-8 string.
 *
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @retval [3] Insufficient parameter. `new_string` is NULL. See `new_string`.
 * @retval [4] Constraint violation. String exceeds the buffer's `limit`. See `buffer_arg->limit`.
 * @retval [5] Failure. Memory allocation (`realloc`) failed.
 */
extern u8t unstdstring_set(
        unstdstring *buffer_arg,
        const i8 *new_string
);


//! [Destructor]
/**
 * @brief Frees the memory allocated for a string buffer (`buffer_arg`).
 * @param buffer_arg (Required) A pointer to a valid `unstdstring` object, which contains the string buffer to be freed.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @retval [3] Insufficient parameter. `buffer_arg->capacity` is 0,
 *             indicating that no memory has been allocated for the data.
 */
extern u8t unstdstring_free(
        unstdstring *buffer_arg
);

/**
 * @brief Resets the string buffer (`buffer_arg`) by clearing its contents and resizing its allocated memory to the default size.
 * @param buffer_arg (Required) A pointer to a valid `unstdstring` object whose buffer is to be reset.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @retval [3] Insufficient parameter. `buffer_arg->capacity` is 0. See `buffer_arg`.
 * @retval [4] Failure. <code>realloc()</code> failed.
 * @note The original buffer remains unchanged if <code>realloc()</code> fails.
 */
extern u8t unstdstring_reset(
        unstdstring *buffer_arg
);

/**
 * @brief Clears the content of the string buffer (`buffer_arg`).
 * @param buffer_arg (Required) A pointer to a valid `unstdstring` object whose string buffer is to be cleared.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @retval [3] Insufficient parameter. `buffer_arg->capacity` is 0 (empty buffer). See `buffer_arg`.
 * @note This function:
 *       - Clears the content of the string buffer (`data`) by setting it to zeros.
 *       - Resets the `length` to 0.
 *       - Does NOT change NOR reallocates the data pointer.
 */
extern u8t unstdstring_clear(
        unstdstring *buffer_arg
);


/**
 * @brief Compares two `unstdstring` objects for equality.
 * @param f_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param s_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the two strings are equal.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The strings were compared successfully.
 * @OutParamValue [2] Insufficient parameter. `f_buffer_arg` or `s_buffer_arg` is NULL. See `f_buffer_arg` or `s_buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `f_buffer_arg` or `s_buffer_arg` is NULL.
 * @OutParamValue [4] Mismatch. One string is empty while the other is not.
 * @note If both strings are empty, they are considered equal.<br>
 *       If the lengths differ, they are considered not equal without comparing contents.<br>
 *       The comparison is case-sensitive and performed using `strcmp`.<br>
 *       Always check `out_error_arg` for reliable status handling.
 */
extern bool unstdstring_equal(
        const unstdstring *const f_buffer_arg,
        const unstdstring *const s_buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Compares a `unstdstring` with a null-terminated C-string buffer for equality.
 * @param f_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param s_buffer_arg (Required) Should be a pointer to a valid, null-terminated C-string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the `unstdstring` and the C-string are equal.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The comparison was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `f_buffer_arg` is NULL. See `f_buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `f_buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. `s_buffer_arg` is NULL. See `s_buffer_arg`.
 * @OutParamValue [5] Insufficient parameter. One of the strings is empty while the other is not.
 * @note This function compares the contents and length of the `unstdstring` and the C-string buffer exactly.
 */
extern bool unstdstring_equal_overload_0(
        const unstdstring *const f_buffer_arg,
        const i8 *const s_buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Compares two `unstdstring` objects for equality, ignoring case (ASCII only).
 * @param f_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param s_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the two strings are equal (case-insensitive).
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. Strings were compared successfully.
 * @OutParamValue [2] Insufficient parameter. `f_buffer_arg` or `s_buffer_arg` is NULL. See `f_buffer_arg` or `s_buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `f_buffer_arg` or `s_buffer_arg` is NULL.
 * @OutParamValue [4] Mismatch. One string is empty while the other is not.
 * @note Case-insensitive comparison is done character by character using `tolower()` on each byte (ASCII only).<br>
 *       If both strings are empty, they are considered equal.<br>
 *       If their lengths differ, they are considered not equal without checking characters.<br>
 *       This function does not support full Unicode case folding—only ASCII.
 */
extern bool unstdstring_equal_ignorecase(
        const unstdstring *const f_buffer_arg,
        const unstdstring *const s_buffer_arg,
        u8t *const out_error_arg
);


/**
 * @brief Case-insensitive comparison between a `unstdstring` and a null-terminated C-string buffer.
 * @param f_buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param s_buffer_arg (Required) Should be a pointer to a valid, null-terminated C-string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the `unstdstring` and the C-string are equal ignoring case (ASCII only).
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The comparison was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `f_buffer_arg` is NULL. See `f_buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `f_buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. `s_buffer_arg` is NULL. See `s_buffer_arg`.
 * @OutParamValue [5] Insufficient parameter. One of the strings is empty while the other is not.
 * @note This function compares the contents of the two strings ignoring ASCII case differences.<br>
 *       It assumes both strings are ASCII or UTF-8 compatible but does not perform full Unicode case folding.
 */
extern bool unstdstring_equal_ignorecase_overload_0(
        const unstdstring *const f_buffer_arg,
        const i8 *const s_buffer_arg,
        u8t *const out_error_arg
);


//! [startswith]
/**
 * @brief Checks whether the given `unstdstring` starts with the specified character.
 * @param buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param checkon_arg (Required) The character to compare against the first character of the string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the string starts with `checkon_arg`.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @note This function compares the first byte of the string against the given character (ANSI/ASCII only).<br>
 *       For multi-byte UTF-8 characters, use a UTF-aware function instead.
 */
extern bool unstdstring_startswith_char(
        const unstdstring *const buffer_arg,
        const i8 checkon_arg,
        u8t *const out_error_arg
);

/**
 * @brief Checks whether the given `unstdstring` starts with the specified character, ignoring case (ASCII only).
 * @param buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param checkon_arg (Required) The character to compare against the first character of the string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the string starts with `checkon_arg`, case-insensitive.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @note This function performs a case-insensitive comparison on the first byte of the string using `tolower()` (ASCII only).<br>
 *       It does not support full Unicode case folding or multi-byte characters.
 */
extern bool unstdstring_startswith_char_ignorecase(
        const unstdstring *const buffer_arg,
        const i8 checkon_arg,
        u8t *const out_error_arg
);


//! [endswith]
/**
 * @brief Checks whether the given `unstdstring` ends with the specified character.
 * @param buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param checkon_arg (Required) The character to compare against the last character of the string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the string ends with `checkon_arg`.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @OutParamValue [5] Failure. The string length is less than 1 and cannot be checked.
 * @note This function compares the last byte of the string against the given character (ANSI/ASCII only).<br>
 *       For multi-byte UTF-8 characters, use a UTF-aware function instead.
 */
extern bool unstdstring_endswith_char(
        const unstdstring *const buffer_arg,
        const i8 checkon_arg,
        u8t *const out_error_arg
);

/**
 * @brief Checks whether the given `unstdstring` ends with the specified character, ignoring case (ASCII only).
 * @param buffer_arg (Required) Should be a pointer to a valid, initialized `unstdstring` object.
 * @param checkon_arg (Required) The character to compare against the last character of the string.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether the string ends with `checkon_arg`, case-insensitive.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @OutParamValue [5] Failure. The string length is less than 1 and cannot be checked.
 * @note This function performs a case-insensitive comparison on the last byte of the string using `tolower()` (ASCII only).<br>
 *       It does not support full Unicode case folding or multi-byte characters.
 */
extern bool unstdstring_endswith_char_ignorecase(
        const unstdstring *const buffer_arg,
        const i8 checkon_arg,
        u8t *const out_error_arg
);

/**
 * @brief Checks whether the given `unstdstring` starts with another `unstdstring`.
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring` to search in.
 * @param checkon_arg (Required) A pointer to a valid, initialized `unstdstring` to check for at the start of `buffer_arg`.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether `buffer_arg` starts with `checkon_arg`.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. Either `buffer_arg` or `checkon_arg` is NULL.
 * @OutParamValue [3] Insufficient parameter. `data` field in one of the input strings is NULL.
 * @OutParamValue [4] Insufficient parameter. One of the strings is empty while the other is not.
 * @OutParamValue [5] Failure. `checkon_arg` is longer than `buffer_arg` and cannot be a prefix.
 * @note This comparison is case-sensitive and only checks raw byte equality.
 */
extern bool unstdstring_startswith_str(
        const unstdstring *const buffer_arg,
        const unstdstring *const checkon_arg,
        u8t *const out_error_arg
);

/**
 * @brief Checks whether the given `unstdstring` ends with another `unstdstring`.
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring` to search in.
 * @param checkon_arg (Required) A pointer to a valid, initialized `unstdstring` to check for at the end of `buffer_arg`.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A boolean (`true` or `false`) indicating whether `buffer_arg` ends with `checkon_arg`.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The check was performed successfully.
 * @OutParamValue [2] Insufficient parameter. Either `buffer_arg` or `checkon_arg` is NULL.
 * @OutParamValue [3] Insufficient parameter. `data` field in one of the input strings is NULL.
 * @OutParamValue [4] Insufficient parameter. One of the strings is empty while the other is not.
 * @OutParamValue [5] Failure. `checkon_arg` is longer than `buffer_arg` and cannot be a suffix.
 * @note This comparison is case-sensitive and performs raw byte comparison.<br>
 *       Use a different function for case-insensitive or UTF-8-aware suffix checking.
 */
extern bool unstdstring_endswith_str(
        const unstdstring *const buffer_arg,
        const unstdstring *const checkon_arg,
        u8t *const out_error_arg
);


//! [tolower]
/**
 * @brief Converts all characters in the given `unstdstring` to lowercase (ASCII only).
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring`. The string is modified in-place.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns This function does not return a value. Use `out_error_arg` to determine success or failure.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. All characters have been converted to lowercase.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL.
 * @OutParamValue [3] Insufficient parameter. The `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @note This function performs in-place conversion of ASCII characters only.<br>
 *       Multi-byte UTF-8 characters will be corrupted if present—ensure the string contains only 8-bit ASCII before use.
 */
extern void unstdstring_tolower_str(
        const unstdstring *const buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Creates a lowercase copy of the given `unstdstring`.
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring` object to copy and convert.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A pointer to a newly allocated `unstdstring` containing the lowercase copy, or `NULL` on failure.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The lowercase copy was created successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. The `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The input string is empty. See `buffer_arg`.
 * @OutParamValue [5] Failure. Memory allocation for the temporary lowercase buffer failed.
 * @OutParamValue [6] Failure. Creation of the new `unstdstring` via `unstdstring_new()` failed.
 * @note The function allocates a temporary C-string, converts each byte to lowercase using `tolower()` (ASCII only),
 *       then constructs a new `unstdstring` with the same `limit` as the original. The original `unstdstring`
 *       is left unmodified.
 */
extern unstdstring *unstdstring_tolower_str_copy(
        const unstdstring *const buffer_arg,
        u8t *const out_error_arg
);


//! [toupper]
/**
 * @brief Converts all characters in the given `unstdstring` to uppercase (ASCII only).
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring`. The string is modified in-place.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns This function does not return a value. Use `out_error_arg` to determine success or failure.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. All characters have been converted to uppercase.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL.
 * @OutParamValue [3] Insufficient parameter. The `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The string is empty. See `buffer_arg`.
 * @note This function performs in-place conversion of ASCII characters only.<br>
 *       Multi-byte UTF-8 characters will be corrupted if present—ensure the string contains only 8-bit ASCII before use.
 */
extern void unstdstring_toupper_str(
        const unstdstring *const buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Creates a uppercase copy of the given `unstdstring`.
 * @param buffer_arg (Required) A pointer to a valid, initialized `unstdstring` object to copy and convert.
 * @param out_error_arg (Optional) A pointer to a `u8t` that will be set to an error code indicating the outcome
 *                      of the operation. Pass `NULL` to ignore.
 * @returns A pointer to a newly allocated `unstdstring` containing the uppercase copy, or `NULL` on failure.
 * @OutParam <strong>out_error_arg</strong> A number (`u8t`) indicating the state of the operation.
 * @OutParamValue [1] Success. The uppercase copy was created successfully.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. The `data` field inside `buffer_arg` is NULL.
 * @OutParamValue [4] Insufficient parameter. The input string is empty. See `buffer_arg`.
 * @OutParamValue [5] Failure. Memory allocation for the temporary uppercase buffer failed.
 * @OutParamValue [6] Failure. Creation of the new `unstdstring` via `unstdstring_new()` failed.
 * @note The function allocates a temporary C-string, converts each byte to uppercase using `toupper()` (ASCII only),
 *       then constructs a new `unstdstring` with the same `limit` as the original. The original `unstdstring`
 *       is left unmodified.
 */
extern unstdstring *unstdstring_toupper_str_copy(
        const unstdstring *const buffer_arg,
        u8t *const out_error_arg
);

//! [is]
//! [isascii]
/**
 * @brief Whether a character is an ASCII control character or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isasciicontrol_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is an ASCII printable character or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isasciiprintable_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is an ASCII extended character or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isasciiextended_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is an ASCII visible character or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isasciivisible_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is an ASCII or not.
 * @note I know i know, it's pretty dump.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isascii_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is within the ranges A-Z, a-z or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isalphabetic_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is within the ranges A-Z, a-z, 0-9 or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isalphanumeric_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is within the range 0-9 or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_isdigit_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is within the ranges A-f, a-f, 0-9 or not.
 * @param buffer_arg Should be an ASCII (u8t) character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_ishex_char(
        const u8t buffer_arg
);

/**
 * @brief Whether a character is a whitespace or not.
 * @param buffer_arg Should be a character.
 * @returns A boolean indicating the state of the operation.
 */
extern bool unstdstring_iswhitespace_char(
        const u8t buffer_arg
);


//! [buffer]
//! [manipulation]
//! [resize]
/**
 * @brief Attempts to grow the internal buffer of a given `unstdstring` by a specified number of encoded bytes.
 * @param buffer_arg (Required) A pointer to a valid `unstdstring` whose buffer should be reallocated and extended.
 * @param bytes_arg (Required) The number of characters to grow by (not bytes—this is multiplied by `encoding_arg`).
 * @param encoding_arg (Required) The encoding multiplier (e.g., 1 for UTF-8, 2 for UTF-16, 4 for UTF-32).
 * @returns A `u8t` error code representing the outcome of the operation.
 * @OutParamValue [1] Success. Buffer was successfully reallocated and zero-initialized.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is `NULL`. See `buffer_arg`.
 * @OutParamValue [3] Invalid argument. `bytes_arg` must be greater than 0.
 * @OutParamValue [4] Unsupported encoding. `encoding_arg` is not recognized or not yet implemented.
 * @OutParamValue [5] Failure. Memory reallocation (`realloc`) failed.
 * @OutParamValue [6] Failure. Zero-initialization of the newly allocated region failed.
 * @note Only `_unstdstring_bufferencoding_UTF8` (value `1`) is currently supported.<br>
 *       UTF-16 and UTF-32 branches are stubbed but not implemented.
 * @warning The `buffer_arg->data` pointer may change after this function due to `realloc`.
 * @warning This function assumes `buffer_arg->length` is accurate for UTF-8 and uses it for size calculations.
 */
extern u8t _unstdstring_grow(
        unstdstring *const buffer_arg,
        const sizet bytes_arg,
        const u8t encoding_arg
);

/**
 * @brief Shrinks the internal buffer of a given `unstdstring` by a specified number of encoded characters.
 * @param buffer_arg (Required) A pointer to a valid `unstdstring` whose buffer should be reduced in size.
 * @param bytes_arg (Required) The number of characters (not bytes) to shrink the buffer by.
 * @param encoding_arg (Required) The encoding multiplier (e.g., 1 for UTF-8, 2 for UTF-16, 4 for UTF-32).
 * @returns A `u8t` error code indicating the outcome of the operation.
 * @OutParamValue [1] Success. Buffer was successfully shrunk and truncated.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is `NULL`. See `buffer_arg`.
 * @OutParamValue [3] Invalid argument. `bytes_arg` must be greater than 0.
 * @OutParamValue [4] Invalid state. The current buffer appears empty.
 * @OutParamValue [5] Invalid range. Attempting to shrink more than the current buffer size.
 * @OutParamValue [6] Unsupported encoding. `encoding_arg` is not recognized or not yet implemented.
 * @OutParamValue [7] Failure. Zeroing memory via `memset` failed.
 * @OutParamValue [8] Failure. Memory reallocation (`realloc`) failed.
 * @note Only `_unstdstring_bufferencoding_UTF8` (value `1`) is currently supported.<br>
 *       UTF-16 and UTF-32 branches are stubbed but not implemented.
 * @warning The pointer `buffer_arg->data` may be modified by `realloc`. Always reassign it after calling this.
 * @warning This function uses `buffer_arg->length` for UTF-8 to determine shrink bounds. This must be accurate.
 */
extern u8t _unstdstring_shrink(
        unstdstring *const buffer_arg,
        const sizet bytes_arg,
        const u8t encoding_arg
);


//! [add / remove]
//! [pop / push]
/**
 * @brief Appends a single character to the end of the string buffer.
 *
 * @details This function adds the character `from_buffer_arg` to the end of the string
 *          represented by `to_buffer_arg`. If the buffer does not have enough capacity
 *          to hold the additional character plus the null terminator, it attempts to grow
 *          the buffer by a fixed amount. After appending, the string is null-terminated.
 *
 * @param to_buffer_arg  Pointer to a valid `unstdstring` buffer to append to.
 * @param from_buffer_arg The character to append.
 *
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is `NULL`. See `buffer_arg`.
 * @retval [3] Failure: buffer growth (`_unstdstring_grow()`) failed.
 */
extern u8t unstdstring_pushback_char(
        unstdstring *restrict const to_buffer_arg,
        const i8 from_buffer_arg
);

/**
 * @brief Removes the last character from the string buffer and returns it.
 *
 * @details This function retrieves and removes the last character from the string
 *          buffer pointed to by `to_buffer_arg`, then null-terminates the updated string.
 *          It also attempts to shrink the buffer by a fixed amount if possible.
 *          The function performs strict null and boundary checks to ensure safe operations.
 *
 * @param to_buffer_arg Pointer to the `unstdstring` buffer from which to pop a character.
 * @param out_error_arg Optional pointer to a `u8t` to receive the status code of the operation.
 *                      May be `NULL` if status is not needed.
 *
 * @returns The removed character (as `i8`) on success, or `0` if an error occurs.
 *
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `buffer_arg` is `NULL`. See `buffer_arg`.
 * @retval [3] Invalid argument: `to_buffer_arg->data` is `NULL`.
 * @retval [4] Buffer is empty: no characters to pop.
 * @retval [5] Buffer shrink operation (`_unstdstring_shrink()`) failed.
 */
extern i8 unstdstring_popback_char(
        unstdstring *restrict const to_buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Appends a string at the end of the string buffer (`to_buffer_arg`).
 * @param to_buffer_arg Should be a pointer to a valid, null-terminated heap-allocated buffer.
 * @param from_buffer_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array / constant buffer.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `to_buffer_arg` is NULL. See `to_buffer_arg`.
 * @retval [3] Insufficient parameter. `to_buffer_arg->data` is NULL. See `to_buffer_arg`.
 * @retval [4] Insufficient parameter. `from_buffer_arg` is NULL. See `from_buffer_arg`.
 * @retval [5] Insufficient parameter. `from_buffer_arg` is an empty string. See `from_buffer_arg`.
 * @retval [6] Failure. <code>_unstdstring_grow()</code> failed.
 */
extern u8t unstdstring_pushback_str(
        unstdstring *const to_buffer_arg,
        const i8 *const from_buffer_arg
);

/**
 * @brief Returns an specific part of the `buffer_arg` as an allocated copy.
 * @param buffer_arg Should be a pointer to a valid, null-terminated heap-allocated buffer.
 * @param start_arg Position of the first character to be copied as a substring.<br>
 *                  If this is greater than or equal to the `buffer_arg` len,<br>
 *                  it returns NULL and throws out_of_range. See `OutParamValue`
 * @param span_arg Number of characters to include in the substring<br>
 *                 (if the string is shorter or `span_arg` is zero or it goes out of index,<br>
 *                 as many characters as possible are read).
 * @returns A pointer to portion of the `buffer_arg` that starts at position `start_arg`<br>
 *          and spans `span_arg` characters. Returns NULL in case of failure.
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @OutParamValue [3] Insufficient parameter. `buffer_arg` is an empty string. See `buffer_arg`.
 * @OutParamValue [4] Failure. `start_arg` out_of_range.
 * @OutParamValue [5] Failure. <code>calloc()</code> failed.
 * @OutParamValue [6] Failure. <code>malloc()</code> failed.
 * @OutParamValue [7] Failure. <code>unstdstring_new()</code> failed.
 */
extern unstdstring *unstdstring_substr_copy(
        unstdstring *const buffer_arg,
        const sizet start_arg,
        const sizet span_arg,
        u8t *const out_error_arg
);

#endif /* UNSTDLIB_UNSTDSTRING_H */
