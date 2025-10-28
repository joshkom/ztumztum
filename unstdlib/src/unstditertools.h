#ifndef UNSTDLIB_UNSTDITERTOOLS_H
#define UNSTDLIB_UNSTDITERTOOLS_H

#include "unstdinttypes.h"
#include "unstdbool.h"

#ifndef unstditertools_foreach_rangeM
/**
 * @brief Create a for loop iteration within a specified range.
 * @attention Always use boundaries like:
 *            <code>
 *            if (i == 0) { break; }
 *            </code>
 *            <br>
 *            Because of how C loops evaluate, they wont work fine with
 *            unsigned data types that iterate over or on 0,
 *            and cause integer overflow, thus resulting in an infinite loop.<br>
 * @note      Negative steps aren't allowed and even if you try to do,<br>
 *            it gets interpreted as positive 1 by default.
 * @param type_arg The type of the variable.
 * @param iterator_arg The variable in which the numbers are gonna get set to.<br><br>
 * @param range_arg A range format via array-initializer which is<br><br>
 *                  <strong><code>{start, end, step}</code></strong>.<br><br>
 *                  <strong>[step]</strong><br>
 *                  Must be non-negative.<br>
 *                  <strong>Maximum step size</strong>: should be less than the size of `u32t` or 2^32.<br>
 *                  <strong>Minimum step size</strong>: 0.<br>
 *                  You are allowed to put any number for start or end as long as it can fit within `type_arg`.
 */
#define unstditertools_foreach_rangeM(type_arg, iterator_arg, ...)                    \
    for (type_arg iterator_arg = ((type_arg[3])__VA_ARGS__)[0];                       \
          ((type_arg[3])__VA_ARGS__)[0] < ((type_arg[3])__VA_ARGS__)[1]               \
          ? iterator_arg <= ((type_arg[3])__VA_ARGS__)[1]                             \
          : iterator_arg >= ((type_arg[3])__VA_ARGS__)[1];                            \
         iterator_arg += (((sizeof((type_arg[3])__VA_ARGS__) / sizeof(type_arg) == 3) \
         && ((s32t[3])__VA_ARGS__)[2] > 0)                                            \
         ? ((type_arg[3])__VA_ARGS__)[0] < ((type_arg[3])__VA_ARGS__)[1]              \
            ? ((s32t[3])__VA_ARGS__)[2]                                               \
            : -((s32t[3])__VA_ARGS__)[2]                                              \
         : ((type_arg[3])__VA_ARGS__)[0] < ((type_arg[3])__VA_ARGS__)[1] ? 1 : -1))
#endif

#endif /* UNSTDLIB_UNSTDITERTOOLS_H */
