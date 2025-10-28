#include "unstdinternal.h"


u8t unstdinternal_check_arg_state(void **arg_arg) {
    if (arg_arg) {
        if (*arg_arg) {
            return UNSTDINTERNAL_ARG_STATE_POINTER_TO_VARIABLE_PASSED;
        } else {
            return UNSTDINTERNAL_ARG_STATE_POINTER_TO_NULL_PASSED;
        }
    } else {
        return UNSTDINTERNAL_ARG_STATE_DIRECT_NULL_PASSED;
    }
}
