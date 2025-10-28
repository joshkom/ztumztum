#ifndef UNSTDLIB_UNSTDIO_H
#define UNSTDLIB_UNSTDIO_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "unstdinttypes.h"
#include "unstdbool.h"


#ifndef unstdio_freadlinesM
//! [macros]
/**
 * @brief Reads lines of bytes from the file pointer `fileptr_arg`
 *        and writes the content of the line to the buffer `line_data_arg`
 *        and the bytes of the line to `line_bytes_arg`.
 * @attention It should be used almost like any other scope-based loop. See `example`.
 * @param line_data_arg Will be declared and assigned.
 * @param line_bytes_arg Will be declared and assigned.
 * @example
 * <pre>
 * @code
 * FILE *fileptr_as_read = openfile(filepath_arg, "r");
 * if (fileptr_as_read == NULL) {
 *     // Error handling
 * }
 *
 * <strong>unstdio_freadlinesM</strong>(line, bytes, fileptr_as_read, {
 *     printf("Retrieved bytes of the current line %zu:\n", bytes);
 *     printf("line content: %s\n", line);
 * })
 * </pre>
 */
#define unstdio_freadlinesM(line_data_arg, line_bytes_arg, fileptr_arg, body_arg)            \
    do {                                                                                     \
        ssizet line_bytes_arg;                                                               \
        sizet len = 0;                                                                       \
        i8 *line_data_arg = NULL;                                                            \
        while ((line_bytes_arg = getline(&line_data_arg, &len, fileptr_arg)) != -1) body_arg \
        free(line_data_arg);                                                                 \
    } while (0)
#endif


//! [file operations]
/**
 * @brief Opens a file with enhanced error handling.
 * @param filepath_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                     containing a path to the desired file.
 * @param mod_arg The usual modes that you would pass to <code>fopen()</code> function.
 * @param <strong>fileptr_arg</strong> Should be a pointer to a NULL FILE handle. Pass NULL to ignore.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `filepath_arg` is NULL. See `filepath_arg`.
 * @retval [3] Insufficient parameter. `filepath_arg` is an empty string. See `filepath_arg`.
 * @retval [4] Insufficient parameter. `mod_arg` is NULL. See `mod_arg`.
 * @retval [5] Insufficient parameter. `mod_arg` is an empty string. See `mod_arg`.
 * @retval [6] Insufficient parameter. `fileptr_arg` is NULL. See `fileptr_arg`.
 * @retval [7] Failure. <code>fopen()</code> failed.
 */
extern u8t unstdio_openfile(
        const i8 *const filepath_arg,
        const i8 *const mod_arg,
        FILE **fileptr_arg
);

/**
 * @brief Checks if a file stream is open/valid.
 * @param fileptr_arg Should be a valid pointer to a FILE handle.
 * @returns True if `fileptr_arg` is open/valid otherwise False.
 */
extern bool unstdio_isfilestreamvalid(const FILE *const fileptr_arg);

/**
 * @brief Closes `fileptr_arg` file descriptor.
 * @param fileptr_arg Should be a valid pointer to a valid FILE handle.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `fileptr_arg` is NULL. See `fileptr_arg`.
 * @retval [3] Insufficient parameter. `fileptr_arg` is invalid or closed.
 *         Further execution might cause double-close thus resulting an UB(undefined behavior). See `fileptr_arg`.
 * @retval [4] Failure. <code>fclose()</code> failed.
 */
extern u8t unstdio_closefile(FILE *const fileptr_arg);

/**
 * @brief Removes a file.
 * @param fileptr_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                     containing a path to the desired file.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [1] Success.
 * @retval [2] Insufficient parameter. `filepath_arg` is NULL. See `filepath_arg`.
 * @retval [3] Insufficient parameter. `filepath_arg` is an empty string. See `filepath_arg`.
 * @retval [4] Failure. <code>unstdio_doesfileexist()</code> failed.
 * @retval [5] Failure. The specified `filepath_arg` doesn't exist.
 * @retval [6] Failure. <code>remove()</code> failed.
 */
extern u8t unstdio_removefile(const i8 *const filepath_arg);

/**
 * @brief Checks whether `filepath_arg` file exists or not.
 * @param fileptr_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                     containing a path to the desired file.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [0] False.
 * @retval [1] True.
 * @retval [2] Insufficient parameter. `filepath_arg` is NULL. See `filepath_arg`.
 * @retval [3] Insufficient parameter. `filepath_arg` is an empty string. See `filepath_arg`.
 * @retval [4] Failure. <code>access()</code> failed.
 */
extern u8t unstdio_doesfileexist(const i8 *const filepath_arg);

/**
 * @brief Checks whether `filepath_arg` is a file or not.
 * @param fileptr_arg Should be a pointer to a valid, null-terminated heap-allocated / c-array buffer
 *                     containing a path to the desired file.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [0] False.
 * @retval [1] True.
 * @retval [2] Insufficient parameter. `filepath_arg` is NULL. See `filepath_arg`.
 * @retval [3] Insufficient parameter. `filepath_arg` is an empty string. See `filepath_arg`.
 * @retval [4] Failure. <code>stat()</code> failed.
 */
extern u8t unstdio_isregularfile(const i8 *const filepath_arg);

/**
 * @brief Gets the size of a file in bytes excluding the null-terminators.
 * @param fileptr_arg Should be a valid pointer to a valid FILE handle.
 * @returns A number (s64t) indicating the state of the operation.
 * @retval [>=0] The number of bytes in the file pointed to by `fileptr_arg`.
 * @retval [-1] Insufficient parameter. `filepath_arg` is NULL. See `filepath_arg`.
 * @retval [-2] Insufficient parameter. `fileptr_arg` is invalid or closed.
 * @retval [-3] Failure. <code>fseek()</code> failed.
 * @retval [-4] Failure. <code>ftell()</code> failed.
 */
extern s64t unstdio_getfilesize(FILE *const fileptr_arg);

#endif //UNSTDLIB_UNSTDIO_H
