#ifndef UNSTDLIB_UNSTDCHAR_COMPAT_H
#define UNSTDLIB_UNSTDCHAR_COMPAT_H

#include "unstdinttypes.h"
#include "unstdbool.h"
#include "_unstdcommon.h"


/**
 * @brief Converts an uppercase ASCII character to lowercase.
 *
 * @param buffer_arg The input character (`i8`).
 *
 * @returns The lowercase equivalent if `buffer_arg` is between 'A' and 'Z', otherwise returns `buffer_arg` unchanged.
 */
i8 unstdchar_compat_tolower8(
        i8 buffer_arg
);

#endif //UNSTDLIB_UNSTDCHAR_COMPAT_H
