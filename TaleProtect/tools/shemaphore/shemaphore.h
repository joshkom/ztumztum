//
// Created by johan on 09.10.25.
//

#ifndef TALESTARS_V63_322_SHEMAPHORE_H
#define TALESTARS_V63_322_SHEMAPHORE_H

#include <unstdinttypes.h>
#include <semaphore.h>
#include <time.h>
#include <stdatomic.h>

typedef enum shemaphore_result_e : s8t {
    SHEMAPHORE_FAILURE = -1,
    SHEMAPHORE_TIMEOUT = 0,
    SHEMAPHORE_ACQUIRED = 1
} shemaphore_result_t;

typedef struct shemaphore_s {
    sem_t sem;
} shemaphore_t;

void shemaphore_init(shemaphore_t *s, u32 initial_value);
void shemaphore_signal(shemaphore_t *s);
void shemaphore_wait(shemaphore_t *s);
shemaphore_result_t shemaphore_wait_timeout(shemaphore_t *s, i32 sec, ssizet nsec);

//! [Expected-value]
typedef struct shemaphore_expected_s {
    sem_t sem;
    atomic_uint val;
} shemaphore_expected_t;

void shemaphore_expected_init(shemaphore_expected_t *s, u32 initial_value);
void shemaphore_expected_signal(shemaphore_expected_t *s, u32 value);
void shemaphore_expected_wait(shemaphore_expected_t *s, u32 expected_value);
shemaphore_result_t shemaphore_expected_wait_timeout(shemaphore_expected_t *s, u32 expected_value, i32 sec, ssizet nsec);

#endif /* TALESTARS_V63_322_SHEMAPHORE_H */
