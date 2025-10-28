#ifndef UNSTDLIB_UNSTDINTERNAL_H
#define UNSTDLIB_UNSTDINTERNAL_H

#include <memory.h>
#include "unstdinttypes.h"

typedef enum : u8t {
    UNSTDINTERNAL_ARG_STATE_DIRECT_NULL_PASSED,
    UNSTDINTERNAL_ARG_STATE_POINTER_TO_NULL_PASSED,
    UNSTDINTERNAL_ARG_STATE_POINTER_TO_VARIABLE_PASSED,
    UNSTDINTERNAL_ARG_STATE_COUNT
} UNSTDINTERNAL_ARG_STATE;


/**
 * @brief Verifies whether `arg_arg` is a NULL pointer or a direct NULL.<br>
 *        If the check is successful, it then determines whether `arg_arg` is<br>
 *        actually pointing to a variable or is just a NULL pointer.
 * @param arg_arg The argument on which the check gets performed on.
 * @returns An enum of type `UNSTDINTERNAL_ARG_STATE`, containing the result of the operation.
 */
u8t unstdinternal_check_arg_state(
        void **arg_arg
);

void unstdinternal_state_based_arg_assignment(
        void **arg_arg,
        const void *value_arg,
        const sizet bytes_arg
);


#endif
