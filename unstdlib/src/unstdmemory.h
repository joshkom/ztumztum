#ifndef UNSTDLIB_UNSTDMEMORY_H
#define UNSTDLIB_UNSTDMEMORY_H

#include "unstdinttypes.h"
#include <stdlib.h>

#define unstdmemory_type_mallocM(type) ((type *)malloc(sizeof(type)))
#define unstdmemory_type_callocM(type) ((type *)calloc(1, sizeof(type)))


//! [find]
/**
 * @brief Finds the index of the first occurrence of a byte (u32t) in a buffer starting from a given position.
 *
 * This function searches for the first occurrence of a word within a buffer starting from a specified position.
 * The search is limited to the range of the buffer starting at the given position to the end of the buffer.
 *
 * @param haystack_arg Pointer to the buffer in which the search is performed.
 * @param haystack_length_arg Length of the buffer in bytes.
 * @param needle_int The u32t value to search for. This value is cast to a single byte before searching.
 * @param position_arg The starting position in the buffer from which the search begins.
 * @param out_error_arg Pointer to a variable that will be set to an error code indicating the result of the operation.
 * @returns The index of the first occurrence of the byte in the buffer starting from `position_arg`, or 0 if not found.
 *          It's best practice to always check for errors. See `out_error_arg`.
 *
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The byte was found at the specified index.
 * @OutParamValue [2] Failure. `haystack_arg` is NULL or `haystack_length_arg` is 0. See `haystack_arg` and `haystack_length_arg`.
 * @OutParamValue [4] Failure. `position_arg` is greater than or equal to `haystack_length_arg`. See `position_arg`.
 * @OutParamValue [6] Failure. The byte was not found in the specified range of the buffer.
 */
sizet unstdmemory_findbyteindex32(
        const u8t *haystack_arg,
        sizet haystack_length_arg,
        u32t needle_int,
        sizet position_arg,
        u8t *const out_error_arg
);

sizet unstdmemory_findbyteindex64(
        const u8t *haystack_arg,
        sizet haystack_length_arg,
        u64t needle,
        sizet needle_length_arg,
        sizet position_arg,
        u8t *const out_error_arg
);

/**
 * @brief Searches for the first occurrence of a byte sequence (`needle`) within another byte sequence (`haystack`).
 *
 * @details This function searches for the `needle` byte sequence starting at a specified position within the `haystack`.
 *          If the `needle` is found, the function returns the index (offset) within the `haystack` where the `needle` begins.
 *
 * @param haystack_arg Pointer to the byte sequence where the search is performed.
 * @param haystack_length_arg Length of the `haystack` byte sequence.
 * @param needle_arg Pointer to the byte sequence to be found within the `haystack`.
 * @param needle_length_arg Length of the `needle` byte sequence.
 * @param position_arg Starting position within the `haystack` to begin the search.
 * @param out_error_arg A number (u8t) indicating the state of the operation. See `out_error_arg`.
 *
 * @returns The index within the `haystack` where the `needle` starts if found, otherwise 0.
 *          The return value is not very reliable for error checking. It's best practice to always check for errors.
 *          See `out_error_arg`.
 *
 * @OutParam <strong>out_error_arg</strong> A number (u8t) indicating the state of the operation.
 * @OutParamValue [1] Success. The `needle` was found within the `haystack`.
 * @OutParamValue [2] Insufficient parameter. `haystack_length_arg` is 0. See `haystack_arg`.
 * @OutParamValue [3] Insufficient parameter. `needle_length_arg` is 0. See `needle_arg`.
 * @OutParamValue [4] Insufficient parameter. `position_arg` is greater than or equal to `haystack_length_arg`. See `position_arg`.
 * @OutParamValue [5] Insufficient parameter. `needle_arg` can not fit in the given haystack from `position_arg`. See `position_arg`.
 * @OutParamValue [6] Failure. The `needle` was not found within the `haystack`.
 */
sizet unstdmemory_findbytesindex(
        const u8t *haystack_arg,
        sizet haystack_length_arg,
        const u8t *needle_arg,
        sizet needle_length_arg,
        sizet position_arg,
        u8t *const out_error_arg
);

/**
 * @brief Erases a specific number of bytes from a buffer starting at a given position.
 *
 * @details This function removes a segment of bytes from a dynamically allocated buffer.
 *
 * @param buffer_arg Pointer to the buffer from which bytes will be erased.
 * @param buffer_length_arg The len of the buffer in bytes.
 * @param position_arg The starting position in the buffer from which bytes will be erased.
 * @param bytes_number_arg The number of bytes to erase from the buffer. If `bytes_number_arg` is `-1`, all bytes from `position_arg` to the end of the buffer are erased.
 * @returns A number (u8t) indicating the state of the operation.
 *
 * @returns [1] Success. The specified bytes were erased and the buffer was updated.
 * @returns [2] Failure. The `position_arg` is out of range. It is greater than or equal to the buffer len. See `position_arg`.
 * @returns [3] Failure. The `bytes_number_arg` is 0, which is an invalid operation. See `bytes_number_arg`.
 */
u8t unstdmemory_erasebytes(
        u8t *buffer_arg,
        sizet buffer_length_arg,
        size_t position_arg,
        sizet bytes_number_arg
);

#endif /* UNSTDLIB_UNSTDMEMORY_H */
