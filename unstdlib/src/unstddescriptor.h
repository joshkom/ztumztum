#ifndef UNSTDLIB_UNSTDDESCRIPTOR_H
#define UNSTDLIB_UNSTDDESCRIPTOR_H

#include "unstdinttypes.h"
#include "unstdbool.h"

/**
 * @brief Checks if a file descriptor is open/valid.
 * @param fd_arg Should be a non-negative integer.
 * @returns True if `fd_arg` is open/valid otherwise False.
 */
extern bool unstddescriptor_isvalidfd(
    const s32t fd_arg
);

/**
 * @brief Sets the file descriptor to non-blocking mode.
 * @param fd_arg Should be a non-negative integer representing a valid file descriptor.
 * @returns A number (u8t) indicating the result of the operation.
 * @retval [1] Success. The file descriptor is now set to non-blocking mode.
 * @retval [2] Insufficient parameter. `fd_arg` is not a valid file descriptor. See `fd_arg`.
 * @retval [3] Failure. <code>fcntl()</code> failed while getting the file descriptor flags.
 * @retval [4] Failure. <code>fcntl()</code> failed while setting the file descriptor to non-blocking mode.
 */
extern u8t unstddescriptor_setfdtononblock(
    const s32t fd_arg
);

#endif /* UNSTDLIB_UNSTDDESCRIPTOR_H */
