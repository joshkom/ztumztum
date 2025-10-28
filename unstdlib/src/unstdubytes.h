#ifndef UNSTDLIB_UNSTDUBYTES_H
#define UNSTDLIB_UNSTDUBYTES_H

#include "unstdinttypes.h"
#include "unstdbool.h"

typedef struct {
    sizet length;
    sizet capacity;
    u8t *data;
} ubytes;

typedef enum {
    UNSTDUBYTES_GROWTH_LINEAR = 1,
    UNSTDUBYTES_GROWTH_EXPONENTIAL = 2
} unstdubytes_growth_mode;

/**
 * @brief Grows the allocated_len of a `ubytes` buffer by the specified number of bytes.
 * @details This function attempts to expand the allocated_len of the given byte buffer to accommodate additional
 *          bytes by reallocating memory.
 * @param buffer_arg A pointer to the `ubytes` structure representing the buffer to be grown.
 * @param size_bytes_arg The number of additional bytes to grow the buffer by.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success. The buffer's allocated_len has been successfully increased.
 * @retval [2] Insufficient parameter. `buffer_arg` is NULL. See `buffer_arg`.
 * @retval [3] Insufficient parameter. `size_bytes_arg` is 0. See `size_bytes_arg`.
 * @retval [4] Failure. <code>realloc()</code> failed.
 * @note This function updates the `allocated_len` field of the `ubytes` structure but does not modify
 *       the `len` field, as it only changes the buffer's allocated_len.
 */
u8t unstdubytes_grow(
        ubytes *const buffer_arg,
        const sizet size_bytes_arg
);

/**
 * @brief Writes bytes from a source buffer to a destination buffer at a specified offset.
 * @param destination_buffer_arg A pointer to the `ubytes` structure representing the destination buffer.
 * @param source_buffer_arg A pointer to the source buffer to be copied.
 * @param source_buffer_size_arg The size (in bytes) of the source buffer to be copied.
 * @param offset_arg The offset in the destination buffer where writing begins.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `destination_buffer_arg` is NULL. See `destination_buffer_arg`.
 * @retval [3] Insufficient parameter. `source_buffer_arg` is NULL. See `source_buffer_arg`.
 * @retval [4] Insufficient parameter. `source_buffer_size_arg` is zero. See `source_buffer_size_arg`.
 * @retval [5] Failure. The sum of `offset_arg` and `source_buffer_size_arg` exceeds the allocated_len of the destination buffer.
 * @note The operation will not proceed if `offset_arg + source_buffer_size_arg` exceeds
 *       `destination_buffer_arg->allocated_len`.
 */
u8t unstdubytes_writebytes(
        ubytes *const destination_buffer_arg,
        const u8t *const source_buffer_arg,
        const sizet source_buffer_size_arg,
        const sizet offset_arg
);


/**
 * @brief Appends bytes from a source buffer to the end of a destination buffer.
 * @param destination_buffer_arg A pointer to the `ubytes` structure representing the destination buffer.
 * @param source_buffer_arg A pointer to the source buffer containing the bytes to append.
 * @param source_buffer_size_arg The size (in bytes) of the source buffer to be appended.
 * @returns A number (`u8t`) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `destination_buffer_arg` is NULL. See `destination_buffer_arg`.
 * @retval [3] Insufficient parameter. `source_buffer_arg` is NULL. See `source_buffer_arg`.
 * @retval [4] Insufficient parameter. `source_buffer_size_arg` is zero. See `source_buffer_size_arg`.
 * @retval [5] Failure. <code>unstdubytes_grow()</code> failed to grow the destination buffer to accommodate the new bytes.
 * @note The function will attempt to grow the destination buffer if the total len
 *       after appending exceeds the current allocated_len.
 */
u8t unstdubytes_appendbytes(
        ubytes *const destination_buffer_arg,
        const u8t *const source_buffer_arg,
        const sizet source_buffer_size_arg
);

u8t unstdubytes_writebytes_autogrow_mode(
        ubytes *const destination_buffer_arg,
        const u8t *const source_buffer_arg,
        const sizet source_buffer_size_arg,
        const sizet offset_arg,
        unstdubytes_growth_mode growth_mode_arg
);

u8t unstdubytes_writebytes_autogrow_linear(
        ubytes *const destination_buffer_arg,
        const u8t *const source_buffer_arg,
        const sizet source_buffer_size_arg,
        const sizet offset_arg
);

sizet unstdubytes_length(
        ubytes *buffer_arg,
        u8t *const out_error_arg
);

bool unstdubytes_set_length(
        ubytes *buffer_arg,
        sizet new_length_arg,
        u8t *const out_error_arg
);

bool unstdubytes_increase_length(
        ubytes *buffer_arg,
        sizet amount_arg,
        u8t *const out_error_arg
);

sizet unstdubytes_remaining(
        ubytes *buffer_arg,
        u8t *const out_error_arg
);

bool unstdubytes_has_remaining(
        ubytes *buffer_arg,
        u8t *const out_error_arg
);

#endif /* UNSTDLIB_UNSTDUBYTES_H */
