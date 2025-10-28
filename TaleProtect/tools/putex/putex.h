#ifndef TALESTARS_V63_322_PUTEX_H
#define TALESTARS_V63_322_PUTEX_H

#include <unstdinttypes.h>
#include <stdatomic.h>

typedef enum putex_result_e : s8t {
    PUTEX_FAILURE = -1,
    PUTEX_TIMEOUT = 0,
    PUTEX_ACQUIRED = 1,
    PUTEX_UNKNOWN = -2
} putex_result_t;

typedef struct putex_s {
    atomic_int val;
} putex_t;

void putex_init(
        putex_t *ptx_arg,
        atomic_int initial_arg
);

void putex_signal(
        putex_t *ptx_arg
);

void putex_wait(
        putex_t *ptx_arg
);

putex_result_t putex_wait_timeout(
        putex_t *ptx_arg,
        i32 sec_arg,
        ssizet nsec_arg
);

//! [putex_expected]
void putex_expected_signal(
        putex_t *ptx_arg,
        atomic_int value_arg
);

void putex_expected_wait(
        putex_t *ptx_arg,
        atomic_int expected_value_arg
);

putex_result_t putex_expected_wait_timeout(
        putex_t *ptx_arg,
        atomic_int expected_value_arg,
        i32 sec_arg,
        ssizet nsec_arg
);


#endif /* TALESTARS_V63_322_PUTEX_H */
