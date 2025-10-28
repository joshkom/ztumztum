#ifndef UNSTDLIB__UNSTDCOMMON_H
#define UNSTDLIB__UNSTDCOMMON_H

#include "unstdinttypes.h"

#if defined(__WIN32) || defined(__WIN64)
#define __SHITDOWS
#error "This library is not intended to be used on Windows platforms."
#endif

#ifdef __STDC__
#ifdef __STDC_VERSION__
#if (__STDC_VERSION__ < 201112L)
#error "Compiler's version should be at least C11."
#endif
#endif
#endif

#define unstdcommon_is_compiler_gccM (!defined(__clang__) \
    && (defined(__GNUC__)                                 \
    || defined(__GNUC_MINOR__)                            \
    || defined(__GNUC_PATCHLEVEL__)))

#define unstdcommon_is_compiler_clangM (defined(__clang__) \
    || defined(__clang_major__)                            \
    || defined(__clang_minor__)                            \
    || defined(__clang_patchlevel__))

#if (!defined(__clang__)              \
    && (defined(__GNUC__)             \
    || defined(__GNUC_MINOR__)        \
    || defined(__GNUC_PATCHLEVEL__)))
#define UNSTDCOMMON_COMPILER_TYPE_GCC
#elif (defined(__clang__)             \
    || defined(__clang_major__)       \
    || defined(__clang_minor__)       \
    || defined(__clang_patchlevel__))
#define UNSTDCOMMON_COMPILER_TYPE_CLANG
#else
#define UNSTDCOMMON_COMPILER_TYPE_UNKNOWN
#endif

#define unstdcommon_returnM(error_arg)     \
    if (out_error_arg) {                   \
        *out_error_arg = error_arg;        \
    }                                      \
    return

#define unstdcommon_return_scopedM(error_arg, return_arg) \
    do {                                                  \
        if (out_error_arg) {                              \
            *out_error_arg = error_arg;                   \
        }                                                 \
        return return_arg;                                \
    } while (0)


/**
 * @brief Checks whether a machine is little-endian or big-endian.
 * @details It first, stores a 2 continues bytes into memory, then fetches the first byte of it.
 * @returns A number (u8t) indicating the state of the operation.
 * @retval [0] little-endian
 * @retval [1] big-endian
 */
u8t unstdcommon_endianness(void);

#endif /* UNSTDLIB__UNSTDCOMMON_H */
