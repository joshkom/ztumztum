#include "../../_test_common.h"

// Headers
#include "../../../src/unstdinternal.h"

//! [check_arg_state]
void test_unstdinternal_check_arg_state(void) {
    // Direct null check
    assert(unstdinternal_check_arg_state(
            NULL) == UNSTDINTERNAL_ARG_STATE_DIRECT_NULL_PASSED);

    // Null pointer check
    u8t *test_null_ptr = NULL;
    assert(unstdinternal_check_arg_state(
            (void **) &test_null_ptr) == UNSTDINTERNAL_ARG_STATE_POINTER_TO_NULL_PASSED);

    // Variable pointer check
    u8t test_var = 0;
    u8t *test_null_ptr_2 = &test_var;
    assert(unstdinternal_check_arg_state(
            (void **) &test_null_ptr_2) == UNSTDINTERNAL_ARG_STATE_POINTER_TO_VARIABLE_PASSED);

    _notify("[+]", "`unstdinternal_check_arg_state()` passed");
}


void test_unstdinternal(void) {
    test_unstdinternal_check_arg_state();

    _notify("[+]", "`unstdinternal` passed");
}
