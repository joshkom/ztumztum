#ifndef UNSTDLIB_UNSTDMEMORY_COMPAT_H
#define UNSTDLIB_UNSTDMEMORY_COMPAT_H

#include "_unstdcommon.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Fills a block of memory with a specified byte value.
 *
 * @param buffer_arg Pointer to the memory block to fill (`void*`).
 * @param char_arg   The value to set (`i32`, low 8 bits used).
 * @param size_arg   Number of bytes to fill (`sizet`).
 *
 * @returns A pointer to the memory block (`u8t*`), same as `buffer_arg`.
 *
 * @note Returns immediately if `size_arg` is `0`.
 */
u8t *unstdmemory_compat_memset(
        void *const buffer_arg,
        i32 char_arg,
        sizet size_arg
);

/**
 * @brief Compares two memory blocks byte by byte.
 *
 * @param f_buffer_arg      Pointer to the first memory block (`const void*`).
 * @param s_buffer_arg      Pointer to the second memory block (`const void*`).
 * @param size_arg Number of bytes to compare (`sizet`).
 *
 * @returns 0 if memory blocks are equal, <0 if f_buffer_arg < s_buffer_arg, >0 if f_buffer_arg > s_buffer_arg.
 *
 * @note Returns immediately if pointers are equal or if either is NULL.
 */
i32 unstdmemory_compat_memcmp(
        const void *const f_buffer_arg,
        const void *const s_buffer_arg,
        sizet size_arg
);

/**
 * @brief Copies a block of memory from source to destination.
 *
 * @param dst_arg    Pointer to the destination memory block (`void*`).
 * @param src_arg    Pointer to the source memory block (`const void*`).
 * @param length_arg Number of bytes to copy (`sizet`).
 *
 * @returns A pointer to the destination memory block (`void*`), same as `dst_arg`.
 *
 * @note Returns immediately if `length_arg` is `0` or if `dst_arg` and `src_arg` are identical.
 */
void *unstdmemory_compat_memcpy(
        void *const dst_arg,
        const void *const src_arg,
        sizet length_arg
);

#ifdef __cplusplus
}
#endif

#endif //UNSTDLIB_UNSTDMEMORY_COMPAT_H
