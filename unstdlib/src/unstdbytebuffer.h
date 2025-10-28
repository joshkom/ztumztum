#ifndef UNSTDLIB_UNSTDBYTEBUFFER_H
#define UNSTDLIB_UNSTDBYTEBUFFER_H

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "unstdubytes.h"

typedef struct {
    ubytes buffer;
    sizet position;
    sizet limit;
} unstdbytebuffer;


/**
 * @brief Initializes a byte buffer with a specified preallocated size and maximum allocated_len.
 *
 * @details This function allocates memory for and initializes a new `unstdbytebuffer` structure. The byte buffer
 *          can optionally preallocate memory for efficient data storage. The buffer's maximum size is constrained
 *          by the provided `max_capacity_arg`.
 *
 * @param preallocate_size_arg The size in bytes to preallocate for the buffer's initial allocated_len. If set to 0,
 *                             no memory will be allocated for the buffer's data initially.
 * @param max_capacity_arg The maximum allowable size in bytes for the buffer. If `preallocate_size_arg` exceeds
 *                         `max_capacity_arg`, the buffer's allocated_len will be capped at `max_capacity_arg`.
 * @param out_error_arg A pointer to a `u8t` variable that will store an error code indicating the operation's
 *                      result. Can be `NULL` if error reporting is not needed.
 *
 * @returns A pointer to the newly created `unstdbytebuffer` structure on success, or `NULL` on failure.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the state of the operation.
 * @OutParamValue [1] Success. The buffer was successfully created and initialized.
 * @OutParamValue [2] Memory allocation failure. Could not allocate memory for the `unstdbytebuffer` structure.
 * @OutParamValue [3] Memory allocation failure for buffer data. Could not allocate the requested preallocated space.
 *
 * @note If the `preallocate_size_arg` is larger than the `max_capacity_arg`, the buffer's allocated_len will
 *       be set to `max_capacity_arg`. If no preallocation is requested (i.e., `preallocate_size_arg == 0`),
 *       the `data` will be initialized to `NULL`.
 *
 * @warning Always check the returned pointer and the value of `out_error_arg` (if provided) to ensure successful initialization.
 */
unstdbytebuffer *unstdbytebuffer_new(
        const sizet preallocate_size_arg,
        const sizet max_capacity_arg,
        u8t *const out_error_arg
);

/**
 * @brief Creates a new unstdbytebuffer with both initial and maximum limit set to `size_arg`.
 * @param size_arg Initial and maximum limit of the buffer.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 * @returns Pointer to a new `unstdbytebuffer` on success, or NULL on failure.
 * @note Internally uses <code>unstdbytebuffer_new()</code>. See its documentation for detailed behavior.
 */
unstdbytebuffer *unstdbytebuffer_new_overload_0(
        const sizet size_arg,
        u8t *const out_error_arg
);

/**
 * @brief Creates a byte buffer from an existing data pointer without taking ownership.
 *
 * @details This function initializes a new `unstdbytebuffer` structure using the provided external
 *          buffer. The `unstdbytebuffer` will use the given data directly without allocating or copying it.
 *          It assumes the memory is valid for the lifetime of the byte buffer and does not attempt to free it.
 *
 * @param buffer_arg A pointer to an existing byte buffer. Must not be `NULL`.
 * @param length_arg The number of bytes available in `buffer_arg`. Must be greater than 0.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns A pointer to the initialized `unstdbytebuffer` structure on success, or `NULL` on failure.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result of the operation.
 * @OutParamValue [1] Success. The buffer was successfully created using the provided data.
 * @OutParamValue [2] Invalid parameter. `buffer_arg` is `NULL`.
 * @OutParamValue [3] Invalid parameter. `length_arg` is zero or negative.
 * @OutParamValue [4] Internal failure. Could not initialize the `unstdbytebuffer` structure.
 *
 * @note This function does not copy or manage the lifetime of `buffer_arg`. The caller is responsible
 *       for ensuring the memory remains valid and is eventually freed.
 *
 * @warning Do not call `unstdbytebuffer_destroy()` on the returned buffer unless you're certain the
 *          original `buffer_arg` should be freed.
 */
unstdbytebuffer *unstdbytebuffer_new_from_data(
        u8t *buffer_arg,
        sizet length_arg,
        u8t *const out_error_arg
);


/**
 * @brief Creates a new `unstdbytebuffer` instance by copying an existing buffer.
 *
 * @details This function allocates and initializes a new `unstdbytebuffer` and fills its internal
 *          buffer with a copy of the data provided in `buffer_arg`, up to `length_arg` bytes.
 *          The internal capacity, length, and limit are all set to `length_arg`.
 *          Memory is safely allocated and copied, with full error checking.
 *
 * @param buffer_arg      Pointer to the source byte array to copy.
 * @param length_arg      Number of bytes to copy from `buffer_arg`.
 * @param out_error_arg   Optional pointer to receive an error/status code. May be `NULL`.
 *
 * @returns Pointer to a newly allocated `unstdbytebuffer` on success, or `NULL` on failure.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result of the operation.
 * @OutParamValue [1] Success. The buffer was successfully created using the provided data.
 * @OutParamValue [2] Invalid parameter. `buffer_arg` is `NULL`.
 * @OutParamValue [3] Invalid parameter. `length_arg` is zero or negative.
 * @OutParamValue [4] Internal failure. Could not initialize the `unstdbytebuffer` structure.
 */
unstdbytebuffer *unstdbytebuffer_new_from_data_copy(
        u8t *buffer_arg,
        sizet length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Evaluates if a byte buffer has enough allocated_len to accommodate additional bytes.
 *
 * @details This function checks whether the buffer can fit a given number of bytes without exceeding
 *          its maximum allocated_len or requiring reallocation.
 *
 * @param buffer_arg A pointer to the `unstdbytebuffer` to evaluate.
 * @param size_bytes_arg The size in bytes to be added to the buffer.
 *
 * @returns A `u8t` value indicating the evaluation result:
 *
 * @retval [0] Failure. The buffer's maximum allocated_len would be exceeded if the additional bytes were added.
 * @retval [1] Success. The buffer can accommodate the additional bytes without exceeding allocated_len.
 * @retval [2] Buffer reallocation required. The buffer has enough maximum allocated_len, but additional space must be allocated.
 *
 * @note This function only evaluates whether the buffer can fit the requested size. It does not perform
 *       any allocation or modification to the buffer itself.
 */
u8t unstdbytebuffer_evaluatefit(
        const unstdbytebuffer *const buffer_arg,
        const sizet size_bytes_arg
);

/**
 * @brief Destroys a `unstdbytebuffer` instance and releases its allocated resources.
 *
 * @details This function frees the internal data buffer and the `unstdbytebuffer` structure itself.
 *          It is safe to call this function if the buffer was properly initialized.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` instance to be destroyed.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result of the operation.
 * @OutParamValue [1] Success. The buffer and its data were successfully freed.
 * @OutParamValue [2] Invalid parameter. `context_arg` is `NULL`.
 *
 * @note After calling this function, the pointer passed as `context_arg` becomes invalid and must not be reused.
 */
void unstdbytebuffer_destroy(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);


/**
 * @brief Retrieves the current read/write position within the byte buffer.
 *
 * @details Returns the current value of the `position` field in the `unstdbytebuffer` structure.
 *          This value represents the current read/write offset within the buffer.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns The current position within the buffer. If `context_arg` is `NULL`, returns 0.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result of the operation.
 * @OutParamValue [1] Success. The position was successfully retrieved.
 * @OutParamValue [2] Invalid parameter. `context_arg` is `NULL`.
 */
sizet unstdbytebuffer_position(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Sets the read/write position within the byte buffer.
 *
 * @details Updates the `position` field in the `unstdbytebuffer` structure to the specified value.
 *          This position determines the offset used for the next read or write operation. Safety
 *          checks ensure the position does not exceed buffer constraints.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` instance.
 * @param position_arg The new position to set within the buffer.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result of the operation.
 * @OutParamValue [1] Success. The position was successfully updated.
 * @OutParamValue [2] Invalid parameter. `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid state. The buffer's data pointer is `NULL`.
 * @OutParamValue [4] Position exceeds max limit. The specified `position_arg` is greater than `limit`.
 * @OutParamValue [5] Position exceeds allocated length. The specified `position_arg` is greater than `capacity`.
 */
void unstdbytebuffer_set_position(unstdbytebuffer *context_arg,
                                  sizet position_arg,
                                  u8t *const out_error_arg
);


/**
 * @brief Retrieves the maximum limit of the buffer.
 *
 * @details Returns the `limit` value, which represents the maximum number of bytes
 *          the buffer can grow to.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg A pointer to a `u8t` variable to store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns The maximum limit of the buffer, or 0 on failure.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result.
 * @OutParamValue [1] Success. The value was retrieved successfully.
 * @OutParamValue [2] Invalid parameter. `context_arg` is `NULL`.
 */
sizet unstdbytebuffer_limit(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Sets the maximum limit for the buffer.
 *
 * @details Updates the `limit` field of the buffer. The new limit must not be less than
 *          the buffer’s current allocated length.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` instance.
 * @param limit_arg The new maximum limit to assign.
 * @param out_error_arg A pointer to a `u8t` variable to store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized status code indicating the result.
 * @OutParamValue [1] Success. The `limit` was updated.
 * @OutParamValue [2] Invalid parameter. `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid state. The buffer's data pointer is `NULL`.
 * @OutParamValue [4] Rejected. The new `limit_arg` is less than `capacity`.
 */
void unstdbytebuffer_set_limit(
        unstdbytebuffer *context_arg,
        sizet limit_arg,
        u8t *const out_error_arg
);

/**
 * @brief Calculates the number of remaining bytes from the current position to the max limit.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns The count of remaining bytes available, or 0 if `context_arg` is `NULL`.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the state of the operation.
 * @OutParamValue [1] Success. The remaining byte count was computed.
 * @OutParamValue [2] Invalid argument. `context_arg` was `NULL`.
 */
sizet unstdbytebuffer_remaining(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Checks if there are remaining bytes available from the current position.
 *
 * @details This function determines whether the current position within the buffer
 *          is less than the maximum limit, indicating that there are bytes
 *          left to read or write. If `limit` is zero (unset), it assumes
 *          unlimited remaining space and returns `true`.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if there are remaining bytes available; otherwise, `false`.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the state of the operation.
 * @OutParamValue [1] Success. The buffer state was checked.
 * @OutParamValue [2] Invalid argument. `context_arg` was `NULL`.
 */
bool unstdbytebuffer_has_remaining(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Retrieves the current length of data stored in the byte buffer.
 *
 * @details Returns the number of bytes currently held in the buffer. If the
 *          buffer pointer or its data pointer is NULL, the function returns 0
 *          and sets an appropriate error code.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns The length of the buffer's data in bytes, or 0 on error.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. Length retrieved successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is NULL.
 * @OutParamValue [3] Buffer data pointer is NULL.
 */
sizet unstdbytebuffer_length(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);


/**
 * @brief Provides a pointer to the raw byte data contained within the buffer.
 *
 * @details Returns the pointer to the internal byte array held by the buffer.
 *          If the buffer or its data pointer is NULL, the function returns NULL
 *          and sets an appropriate error code.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns A pointer to the byte buffer's internal data, or NULL on error.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. Data pointer returned successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is NULL.
 * @OutParamValue [3] Buffer data pointer is NULL.
 */
u8t *unstdbytebuffer_bytes(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Resets the current position of the byte buffer to the beginning.
 *
 * @details This function sets the `position` field of the buffer to zero,
 *          effectively rewinding the buffer so that subsequent read/write
 *          operations start from the beginning again.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The position was successfully reset to zero.
 * @OutParamValue [2] Invalid argument: `context_arg` is NULL.
 */
void unstdbytebuffer_rewind(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Compacts the byte buffer by discarding the bytes before the current position and shifting the remaining bytes to the start.
 *
 * @details This function moves the unread bytes (from the current `position` to the end of the allocated buffer)
 *          to the beginning of the buffer's data array. It then updates the buffer's length to reflect the new size
 *          of the remaining data and resets the position to the end of the remaining data. This operation effectively
 *          frees up space at the end of the buffer for new data to be written while preserving unread data.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to compact.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The buffer was successfully compacted.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Invalid position: current position is beyond allocated length.
 * @OutParamValue [5] Failed to update buffer length after compaction.
 *
 * @note After compaction, the buffer's `position` is set to the new length of remaining data.
 */
void unstdbytebuffer_compact(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Advances the current position in the byte buffer by a specified length, effectively skipping over bytes.
 *
 * @details This function moves the buffer's current position forward by `length_arg` bytes. It performs
 *          bounds checking to ensure the new position does not exceed the buffer's maximum limit or allocated length.
 *          It also increases the buffer's length accordingly to accommodate the skipped bytes, which is useful when
 *          reserving space for future writes.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance whose position will be advanced.
 * @param length_arg The number of bytes to skip forward in the buffer.
 * @param out_error_arg Pointer to a `u8t` variable that will store an error code indicating the operation's result.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The position was successfully advanced by `length_arg`.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Position overflow: advancing position would exceed maximum limit.
 * @OutParamValue [5] Position overflow: advancing position would exceed allocated length.
 * @OutParamValue [6] Failed to increase buffer length to accommodate skipped bytes.
 *
 * @note This function can be used to reserve space in the buffer by skipping bytes, which can be later
 *       overwritten or used as padding.
 */
void unstdbytebuffer_skip(
        unstdbytebuffer *context_arg,
        sizet length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Resets the byte buffer for reuse by clearing its length and resetting the position.
 *
 * @details This function sets the internal buffer length to zero and resets the read/write
 *          position to the start of the buffer. It prepares the buffer for new data without
 *          reallocating memory. The underlying data pointer remains valid.
 *
 *          If resetting the buffer length fails, an error code is returned and no changes
 *          are made to the position.
 *
 * @param context_arg   Pointer to the `unstdbytebuffer` instance to reset.
 * @param out_error_arg Optional pointer to a `u8t` to receive an error/status code.
 *                      May be `NULL` if status is not needed.
 *
 * @OutParam <strong>out_error_arg</strong> Status codes:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer: data pointer is `NULL`.
 * @OutParamValue [4] Failed to reset buffer length.
 *
 * @note Clearing the buffer does not free its allocated memory, so subsequent writes can reuse the existing allocation.
 */
void unstdbytebuffer_reuse(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a 32-bit unsigned integer to the byte buffer in little-endian order.
 *
 * @details This function writes the given 32-bit unsigned integer (`buffer_arg`) into the buffer
 *          at the current position. It ensures there is enough limit in the buffer to write
 *          4 bytes, growing the buffer if necessary. The position is advanced by 4 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The 32-bit unsigned integer value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The 32-bit unsigned integer was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The integer is written in little-endian byte order, with the least significant byte first.
 */
bool unstdbytebuffer_write_uint32(
        unstdbytebuffer *context_arg,
        u32t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a 32-bit signed integer to the byte buffer in little-endian order.
 *
 * @details This function writes the given 32-bit signed integer (`buffer_arg`) into the buffer
 *          at the current position. It ensures there is enough limit in the buffer to write
 *          4 bytes, growing the buffer if necessary. The position is advanced by 4 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The 32-bit signed integer value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The 32-bit signed integer was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The integer is written in little-endian byte order, with the least significant byte first.
 */
bool unstdbytebuffer_write_int32(
        unstdbytebuffer *context_arg,
        s32t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a 64-bit unsigned integer to the byte buffer in little-endian order.
 *
 * @details This function writes the given 64-bit unsigned integer (`buffer_arg`) into the buffer
 *          at the current position. It ensures there is enough limit in the buffer to write
 *          8 bytes, growing the buffer if necessary. The position is advanced by 8 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The 64-bit unsigned integer value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The 64-bit unsigned integer was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The integer is written in little-endian byte order, with the least significant byte first.
 */
bool unstdbytebuffer_write_uint64(
        unstdbytebuffer *context_arg,
        u64t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a 64-bit signed integer to the byte buffer in little-endian order.
 *
 * @details This function writes the given 64-bit signed integer (`buffer_arg`) into the buffer
 *          at the current position. It ensures there is enough limit in the buffer to write
 *          8 bytes, growing the buffer if necessary. The position is advanced by 8 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The 64-bit signed integer value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The 64-bit signed integer was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The integer is written in little-endian byte order, with the least significant byte first.
 */
bool unstdbytebuffer_write_int64(
        unstdbytebuffer *context_arg,
        s64t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a single byte to the byte buffer.
 *
 * @details This function writes the given byte (`buffer_arg`) into the buffer at the current
 *          position. It ensures there is enough limit in the buffer to write 1 byte,
 *          growing the buffer if necessary. The position is advanced by 1 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The byte value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The byte was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 */
bool unstdbytebuffer_write_byte(
        unstdbytebuffer *context_arg,
        uint8_t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a 64-bit unsigned integer to the byte buffer using variable-length encoding.
 *
 * @details This function writes the given 64-bit unsigned integer (`buffer_arg`) into the buffer
 *          using a variable-length encoding scheme (7 bits per byte, with the MSB as a continuation flag).
 *          It writes one or more bytes depending on the magnitude of the integer.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The 64-bit unsigned integer value to write into the buffer using variable-length encoding.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The variable-length unsigned integer was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Failed to write byte(s) to the buffer.
 *
 * @note The encoding uses 7 bits per byte for data and the MSB as a continuation flag.
 */
bool unstdbytebuffer_write_vuint(
        unstdbytebuffer *context_arg,
        u64t buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a boolean value to the byte buffer as a specific 32-bit unsigned integer.
 *
 * @details This function writes a 32-bit unsigned integer to the buffer representing the boolean value:
 *          - `true` is encoded as `0xBADBABE5`
 *          - `false` is encoded as `0xBADBEEF5`
 *          It uses the `unstdbytebuffer_write_uint32` function internally.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The boolean value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status,
 *                                         as set by `unstdbytebuffer_write_uint32`.
 *
 * @note This encoding uses custom 32-bit constants to represent boolean values rather than
 *       a single byte.
 */
bool unstdbytebuffer_write_bool(
        unstdbytebuffer *context_arg,
        bool buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a double-precision floating-point value to the byte buffer.
 *
 * @details This function writes the given double (`buffer_arg`) into the buffer at the current
 *          position as an 8-byte sequence. It ensures there is enough limit in the buffer,
 *          growing it if necessary. The position is advanced by 8 after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg The double value to write into the buffer.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. The double was written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Buffer max limit exceeded by the write operation.
 * @OutParamValue [5] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [6] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The double value is written in the platform's native binary representation and endianness.
 */
bool unstdbytebuffer_write_double(
        unstdbytebuffer *context_arg,
        double buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes multiple bytes to the byte buffer.
 *
 * @details This function writes `length_arg` bytes from the given buffer (`buffer_arg`)
 *          into the byte buffer at the current position. It ensures there is enough limit
 *          in the buffer to write all bytes, growing the buffer if necessary.
 *          The position is advanced by `length_arg` after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg Pointer to the source byte array to write into the buffer.
 * @param length_arg Number of bytes to write from `buffer_arg`.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. All bytes were written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Invalid argument: `length_arg` is zero (nothing to write).
 * @OutParamValue [5] Invalid argument: buffer_arg is NULL while length_arg > 0.
 * @OutParamValue [6] Buffer max limit would be exceeded by the write operation.
 * @OutParamValue [7] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [8] Failed to increase the buffer's length to reflect the written data.
 *
 * @note This function uses `unstdbytebuffer_write_bytes_internal` to perform the actual copying.
 */
bool unstdbytebuffer_write_bytes(
        unstdbytebuffer *context_arg,
        const uint8_t *buffer_arg,
        sizet length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes a `ubytes` structure into a `unstdbytebuffer`.
 *
 * @param context_arg A pointer to the `unstdbytebuffer` structure where the data will be written.
 * @param source_buffer_arg A pointer to the `ubytes` structure containing the data to write.
 * @param out_error_arg A pointer to a `u8t` variable that will store the result of the operation.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns A boolean (`bool`) indicating the state of the operation.
 *
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Insufficient parameter. `context_arg` is NULL. See `context_arg`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Invalid argument: `length_arg` is zero (nothing to write).
 * @OutParamValue [5] Invalid argument: buffer_arg is NULL while length_arg > 0.
 * @OutParamValue [6] Buffer max limit would be exceeded by the write operation.
 * @OutParamValue [7] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [8] Failed to increase the buffer's length to reflect the written data.
 *
 * @note The function appends the data from the `source_buffer_arg` into the `context_arg` buffer,
 *       and adjusts the position and allocation accordingly.
 *       Furthermore, it redirects your call to `unstdbytebuffer_write_bytes` internally
 *       in order to perform the actual copying.
 */
bool unstdbytebuffer_writeubytes(
        unstdbytebuffer *const context_arg,
        const ubytes *const source_buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Writes multiple bytes from a specified offset in the source buffer to the byte buffer.
 *
 * @details This function writes `length_arg` bytes from the source buffer (`buffer_arg`),
 *          starting at `offset_arg`, into the byte buffer at the current position.
 *          It ensures there is enough limit in the buffer to write all bytes,
 *          growing the buffer if necessary. The position is advanced by `length_arg` after writing.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to write to.
 * @param buffer_arg Pointer to the source byte array.
 * @param offset_arg The offset within `buffer_arg` from which to start writing.
 * @param length_arg Number of bytes to write from `buffer_arg` starting at `offset_arg`.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the write operation succeeded; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> A byte-sized value representing the operation status.
 * @OutParamValue [1] Success. All bytes were written successfully.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Invalid argument: `length_arg` is zero (nothing to write).
 * @OutParamValue [5] Invalid argument: buffer_arg is NULL while length_arg > 0.
 * @OutParamValue [6] Buffer max limit would be exceeded by the write operation.
 * @OutParamValue [7] Failed to grow the underlying buffer to accommodate new data.
 * @OutParamValue [8] Failed to increase the buffer's length to reflect the written data.
 *
 * @note This function uses `unstdbytebuffer_write_bytes_internal` to perform the actual copying,
 *       starting at the specified offset within the source buffer.
 */
bool unstdbytebuffer_write_bytes_offset(
        unstdbytebuffer *context_arg,
        const u8t *buffer_arg,
        sizet offset_arg,
        sizet length_arg,
        u8t *const out_error_arg
);


/**
 * @brief Writes a length-prefixed byte array into the buffer.
 *
 * @details This function first writes the length of the array using `unstdbytebuffer_write_vuint`,
 *          then writes the actual bytes from `buffer_arg` starting at `offset_arg` for
 *          `length_arg` bytes. It ensures the buffer is valid and that enough capacity
 *          is available.
 *          
 * @attention Error values are propagated.         
 *
 * @param context_arg     Pointer to the `unstdbytebuffer` instance where data will be written.
 * @param buffer_arg      Pointer to the byte array from which data will be read.
 * @param offset_arg      Offset into `buffer_arg` from where to begin copying data.
 * @param length_arg      Number of bytes to write from the offset position.
 * @param out_error_arg   Optional pointer to a `u8t` to receive error/status codes. Can be `NULL`.
 *
 * @returns A `bool` indicating the success or failure of the operation.
 *
 * @retval [1]     Success.
 * @retval [2]     Invalid argument: `context_arg` is `NULL`. See `context_arg`.
 * @retval [3]     Invalid buffer: `context_arg->buffer.data` is `NULL` but capacity is non-zero.
 * @retval [10<]   Failure while writing length using `unstdbytebuffer_write_vuint`.
 *                 Use: sub_error = retval - 10; See `unstdbytebuffer_write_vuint` for sub-codes.
 * @retval [20<]   Failure while writing bytes using `unstdbytebuffer_write_bytes`.
 *                 Use: sub_error = retval - 20; See `unstdbytebuffer_write_bytes` for sub-codes.
 *
 * @note This function relies on `unstdbytebuffer_write_vuint` for length encoding
 *       and `unstdbytebuffer_write_bytes` for writing the actual bytes.
 */
bool unstdbytebuffer_write_bytearray(
        unstdbytebuffer *context_arg,
        const u8t *buffer_arg,
        u64t offset_arg,
        u64t length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a 32-bit unsigned integer from the byte buffer in little-endian order.
 *
 * @details This function attempts to read a 4-byte unsigned integer from the buffer at the current
 *          read position. The value is interpreted using little-endian byte order. Upon successful
 *          read, the internal buffer position is advanced by 4 bytes. The function performs
 *          validation on the input arguments, buffer state, and ensures that the read operation
 *          stays within the defined limit and bounds of the buffer.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance from which to read.
 * @param out_error_arg Optional pointer to a `u8t` variable to receive an error code
 *                      indicating the result of the operation. Can be `NULL` if error
 *                      reporting is not required.
 *
 * @returns The 32-bit unsigned integer value read from the buffer if successful; otherwise, returns 0.
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: `data` is `NULL`.
 * @OutParamValue [4] Read operation exceeds the configured `limit`.
 * @OutParamValue [5] Read operation exceeds the `capacity` of the buffer.
 * @OutParamValue [6] Read operation exceeds the current `length` of the buffer.
 */
u32t unstdbytebuffer_read_uint32(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);


/**
 * @brief Reads a 32-bit signed integer from the byte buffer in little-endian order.
 *
 * @details This function attempts to read a 4-byte signed integer from the buffer at the current
 *          read position. The value is interpreted using little-endian byte order. Upon successful
 *          read, the internal buffer position is advanced by 4 bytes. The function performs
 *          validation on the input arguments, buffer state, and ensures that the read operation
 *          stays within the defined limit and bounds of the buffer.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance from which to read.
 * @param out_error_arg Optional pointer to a `u8t` variable to receive an error code
 *                      indicating the result of the operation. Can be `NULL` if error
 *                      reporting is not required.
 *
 * @returns The 32-bit signed integer value read from the buffer if successful; otherwise, returns 0.
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: `data` is `NULL`.
 * @OutParamValue [4] Read operation exceeds the configured `limit`.
 * @OutParamValue [5] Read operation exceeds the `capacity` of the buffer.
 * @OutParamValue [6] Read operation exceeds the current `length` of the buffer.
 */
s32t unstdbytebuffer_read_int32(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a 64-bit unsigned integer from the byte buffer in little-endian order.
 *
 * @details This function reads an 8-byte unsigned integer from the buffer starting at the current
 *          read position. The bytes are interpreted in little-endian format. After a successful
 *          read, the internal read position is advanced by 8 bytes. The function ensures that all
 *          reads stay within the specified buffer limit, allocation, and length, and performs
 *          necessary validity checks on the input arguments and buffer state.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance from which to read.
 * @param out_error_arg Optional pointer to a `u8t` variable to receive an error code
 *                      indicating the result of the operation. Can be `NULL` if error
 *                      reporting is not required.
 *
 * @returns The 64-bit unsigned integer value read from the buffer if successful; otherwise, returns 0.
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: `data` is `NULL`.
 * @OutParamValue [4] Read operation exceeds the configured `limit`.
 * @OutParamValue [5] Read operation exceeds the `capacity` of the buffer.
 * @OutParamValue [6] Read operation exceeds the current `length` of the buffer.
 */
u64t unstdbytebuffer_read_uint64(
        unstdbytebuffer *buffer_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a 64-bit signed integer from the byte buffer in little-endian order.
 *
 * @details This function reads an 8-byte signed integer from the buffer starting at the current
 *          read position. The bytes are interpreted in little-endian format. After a successful
 *          read, the internal read position is advanced by 8 bytes. The function ensures that all
 *          reads stay within the specified buffer limit, allocation, and length, and performs
 *          necessary validity checks on the input arguments and buffer state.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance from which to read.
 * @param out_error_arg Optional pointer to a `u8t` variable to receive an error code
 *                      indicating the result of the operation. Can be `NULL` if error
 *                      reporting is not required.
 *
 * @returns The 64-bit signed integer value read from the buffer if successful; otherwise, returns 0.
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: `data` is `NULL`.
 * @OutParamValue [4] Read operation exceeds the configured `limit`.
 * @OutParamValue [5] Read operation exceeds the `capacity` of the buffer.
 * @OutParamValue [6] Read operation exceeds the current `length` of the buffer.
 */
s64t unstdbytebuffer_read_int64(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a boolean value from the byte buffer.
 *
 * @details This function reads a 32-bit unsigned integer from the buffer and interprets
 *          it as a boolean. A value equal to `0xBADBABE5` is considered `true`, otherwise `false`.
 *          It performs checks for context validity, buffer state, and boundary conditions
 *          within the `unstdbytebuffer_read_uint32` function.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to read from.
 * @param out_error_arg Pointer to a `u8t` variable to receive an error code.
 *                      Can be `NULL` if error reporting is not needed.
 *
 * @returns `true` if the read value equals `0xBADBABE5`; `false` otherwise.
 *
 * @OutParam <strong>out_error_arg</strong> Operation status code from `unstdbytebuffer_read_uint32`:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer state: data pointer is `NULL`.
 * @OutParamValue [4] Read operation would exceed the maximum limit of the buffer.
 * @OutParamValue [5] Read operation would exceed the allocated length of the buffer.
 */
bool unstdbytebuffer_read_bool(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a 64-bit IEEE-754 double-precision floating-point number from the buffer.
 *
 * @details This function reads 8 bytes from the buffer at the current position and interprets them
 *          as a `double`. It performs boundary and safety checks to ensure valid access, including
 *          buffer presence, limit, allocation bounds, and logical buffer length. The buffer’s
 *          read position is incremented by 8 on success.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance containing serialized data.
 * @param out_error_arg Optional pointer to a `u8t` to store the resulting error status.
 *                      May be `NULL` if the caller does not need error feedback.
 *
 * @returns The decoded `double` value from the buffer. On failure, returns `0.0` and sets
 *          an appropriate error code via `out_error_arg`.
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer: `data` is `NULL`.
 * @OutParamValue [4] Read would exceed `limit`.
 * @OutParamValue [5] Read would exceed `capacity`.
 * @OutParamValue [6] Read would exceed logical `length`.
 */
double unstdbytebuffer_read_double(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a single byte (`u8t`) from the buffer at the current read position.
 *
 * @details This function extracts 1 byte from the buffer, ensuring all bounds and memory safety
 *          checks are satisfied. It validates the buffer state and prevents overread beyond the
 *          buffer’s `limit`, `capacity`, or logical `length`. On success, the byte
 *          is returned and the internal `position` is incremented by 1.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance used for reading.
 * @param out_error_arg Optional pointer to a `u8t` used to store the result status code.
 *                      Can be `NULL` if status feedback is not required.
 *
 * @returns The byte read from the buffer. Returns `0` on failure, with an error code
 *          written to `out_error_arg` (if not `NULL`).
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer: `data` is `NULL`.
 * @OutParamValue [4] Read would exceed `limit`.
 * @OutParamValue [5] Read would exceed `capacity`.
 * @OutParamValue [6] Read would exceed logical `length`.
 */
u8t unstdbytebuffer_read_byte(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a variable-length unsigned integer (`u64t`) from the buffer using LEB128 encoding.
 *
 * @details This function decodes a variable-length unsigned integer (varuint) encoded with LEB128-style
 *          continuation bytes. It reads up to `maxv_arg` bytes (defaults to 4 if zero), using the high bit
 *          (0x80) as a continuation flag. Each 7 bits of payload are shifted and accumulated into the
 *          resulting `u64t` value. Safety checks are enforced during each byte read.
 *
 * @param context_arg Pointer to a valid `unstdbytebuffer` instance used for reading.
 * @param maxv_arg    Maximum number of bytes to read. If 0, a default of 4 bytes is used.
 * @param out_error_arg Optional pointer to a `u8t` used to store the result status code.
 *                      Can be `NULL` if status feedback is not required.
 *
 * @returns The decoded unsigned integer value. Returns `0` on failure, with an error code
 *          written to `out_error_arg` (if not `NULL`).
 *
 * @OutParam <strong>out_error_arg</strong> Error status code indicating the result of the operation:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Read error: underlying `unstdbytebuffer_read_byte()` failed.
 * @OutParamValue [4] Malformed varuint: exceeded `maxv_arg` without completing a value.
 */
u64t unstdbytebuffer_read_vuint(
        unstdbytebuffer *context_arg,
        u8t maxv_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a sequence of bytes from the buffer without copying.
 *
 * @details This function returns a pointer to a contiguous block of `length_arg` bytes
 *          starting at the current position in the buffer. The buffer's internal position
 *          is advanced by `length_arg` after a successful read.
 *          It performs bounds checking against the buffer's limit, allocated length,
 *          and current length to prevent out-of-bounds access.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to read from.
 * @param length_arg  Number of bytes to read.
 * @param out_error_arg Optional pointer to a `u8t` to receive an error/status code.
 *                      May be `NULL` if status is not needed.
 *
 * @returns Pointer to the start of the requested byte sequence within the buffer, or
 *          `NULL` if an error occurs (e.g., invalid arguments or insufficient remaining bytes).
 *
 * @OutParam <strong>out_error_arg</strong> Status code describing the operation result:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer: data pointer is `NULL`.
 * @OutParamValue [4] Read exceeds maximum allowed limit.
 * @OutParamValue [5] Read exceeds allocated buffer length.
 * @OutParamValue [6] Read exceeds current buffer length.
 */
u8t *unstdbytebuffer_read_bytes(
        unstdbytebuffer *context_arg,
        sizet length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a sequence of bytes from the buffer and returns a newly allocated copy.
 *
 * @details This function allocates memory for a copy of `length_arg` bytes starting at
 *          the current position in the buffer, copies the data into this new buffer,
 *          advances the internal position by `length_arg`, and returns the pointer
 *          to the copied data. The caller is responsible for freeing the returned pointer.
 *          It performs thorough bounds and validity checks to prevent invalid memory access.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` instance to read from.
 * @param length_arg  Number of bytes to copy.
 * @param out_error_arg Optional pointer to a `u8t` to receive an error/status code.
 *                      May be `NULL` if status is not needed.
 *
 * @returns Pointer to a newly allocated buffer containing the copied bytes, or
 *          `NULL` if an error occurs or memory allocation fails.
 *
 * @OutParam <strong>out_error_arg</strong> Status code describing the operation result:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is `NULL`.
 * @OutParamValue [3] Invalid buffer: data pointer is `NULL`.
 * @OutParamValue [4] Read exceeds maximum allowed limit.
 * @OutParamValue [5] Read exceeds allocated buffer length.
 * @OutParamValue [6] Read exceeds current buffer length.
 * @OutParamValue [7] Memory allocation failure.
 */
u8t *unstdbytebuffer_read_bytes_copy(
        unstdbytebuffer *context_arg,
        sizet length_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a length-prefixed byte array slice from the buffer without copying.
 *
 * @details This function reads a variable-length unsigned integer (up to 5 bytes) that specifies
 *          the length of the byte array to read. Then, it reads a slice of bytes of the specified
 *          length from the current buffer position without copying the data, advancing the position
 *          accordingly. The returned `ubytes` struct contains a pointer to this slice along with
 *          its length. The caller must NOT free the returned data pointer, as it references
 *          the internal buffer data.
 *
 * If any error occurs (invalid context, length reading failure, or insufficient buffer),
 * the function returns an empty `ubytes` struct with zero length and NULL data pointer.
 *
 * @param context_arg Pointer to the `unstdbytebuffer` to read from.
 * @param out_error_arg Optional pointer to a `u8t` to receive an error/status code.
 *                      May be NULL if status is not needed.
 *
 * @returns
 *          - On success: a `ubytes` struct with `data` pointing to the slice inside the buffer,
 *            and `length` set to the number of bytes read.
 *          - On failure: a zero-initialized `ubytes` struct (length = 0, data = NULL).
 *
 * @OutParam <strong>out_error_arg</strong> Status code:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is NULL.
 * @OutParamValue [3] Failed to read length prefix (varuint).
 * @OutParamValue [4] Failed to read the byte slice (insufficient data).
 */
ubytes unstdbytebuffer_read_bytearray(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

/**
 * @brief Reads a length-prefixed byte array from the buffer and returns a newly allocated copy.
 *
 * @details This function reads a variable-length unsigned integer (up to 5 bytes) representing
 *          the length of the byte array. It then allocates memory and copies the specified
 *          number of bytes from the buffer starting at the current position. The internal
 *          position is advanced by the length read. The caller is responsible for freeing
 *          the returned data pointer.
 *
 *          It performs validation checks on the buffer and the requested length to avoid
 *          out-of-bounds memory access. If any error occurs, the function returns a zeroed
 *          `ubytes` struct (length = 0, data = NULL).
 *
 * @param context_arg   Pointer to the `unstdbytebuffer` to read from.
 * @param out_error_arg Optional pointer to a `u8t` to receive an error/status code.
 *                      May be NULL if status is not required.
 *
 * @returns
 *          - On success: a `ubytes` struct containing a pointer to a newly allocated copy
 *            of the byte array and its length.
 *          - On failure: a zero-initialized `ubytes` struct with `data` = NULL and `length` = 0.
 *
 * @OutParam <strong>out_error_arg</strong> Status codes:
 * @OutParamValue [1] Success.
 * @OutParamValue [2] Invalid argument: `context_arg` is NULL.
 * @OutParamValue [3] Invalid buffer: data pointer is NULL.
 * @OutParamValue [4] Failed to read length prefix (varuint).
 * @OutParamValue [5] Failed to read byte array copy (insufficient data).
 */
ubytes unstdbytebuffer_read_bytearray_copy(
        unstdbytebuffer *context_arg,
        u8t *const out_error_arg
);

#endif /* UNSTDLIB_UNSTDBYTEBUFFER_H */
