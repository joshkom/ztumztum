#include "shemaphore.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include <errno.h>
#include <stdio.h>

void shemaphore_init(shemaphore_t *s, u32 initial_value) {
    if (sem_initM(&s->sem, 0, initial_value) != 0) {
        LOGE("(Tools::Shemaphore) sem_init failed: %d", errno);
    }
}

void shemaphore_signal(shemaphore_t *s) {
    if (sem_postM(&s->sem) != 0) {
        LOGE("(Tools::Shemaphore) sem_post failed: %d", errno);
    }
}

void shemaphore_wait(shemaphore_t *s) {
    while (sem_waitM(&s->sem) != 0) {
        if (errno == EINTR) continue;
        LOGE("(Tools::Shemaphore) sem_wait failed: %d", errno);
    }
}

shemaphore_result_t shemaphore_wait_timeout(shemaphore_t *s, i32 sec, ssizet nsec) {
    struct timespec ts;
    if (secure_syscall_v2M(SYS_clock_gettime, 2, CLOCK_REALTIME, &ts) != 0) {
        LOGE("(Tools::Shemaphore) clock_gettime failed via secure_syscallM");
        return SHEMAPHORE_FAILURE;
    }

    ts.tv_sec += sec;
    ts.tv_nsec += nsec;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec += 1;
        ts.tv_nsec -= 1000000000;
    }

    while (1) {
        int r = sem_timedwaitM(&s->sem, &ts);
        if (r == 0) return SHEMAPHORE_ACQUIRED;
        if (errno == ETIMEDOUT) return SHEMAPHORE_TIMEOUT;
        if (errno == EINTR) continue;
        LOGE("(Tools::Shemaphore) sem_timedwait failed: %d", errno);
        return SHEMAPHORE_FAILURE;
    }
}

// --- Expected-value variant ---

void shemaphore_expected_init(shemaphore_expected_t *s, u32 initial_value) {
    atomic_store(&s->val, initial_value);
    if (sem_initM(&s->sem, 0, 0) != 0) {
        LOGE("(Tools::Shemaphore) sem_init failed: %d", errno);
    }
}

void shemaphore_expected_signal(shemaphore_expected_t *s, u32 value) {
    atomic_store(&s->val, value);
    if (sem_postM(&s->sem) != 0) {
        LOGE("(Tools::Shemaphore) sem_post failed: %d", errno);
    }
}

void shemaphore_expected_wait(shemaphore_expected_t *s, u32 expected_value) {
    while (atomic_load(&s->val) != expected_value) {
        shemaphore_wait((shemaphore_t *) &s->sem);
    }
}

shemaphore_result_t
shemaphore_expected_wait_timeout(shemaphore_expected_t *s, u32 expected_value, i32 sec,
                                 ssizet nsec) {
    while (atomic_load(&s->val) != expected_value) {
        shemaphore_result_t r = shemaphore_wait_timeout((shemaphore_t *) &s->sem, sec, nsec);
        if (r != SHEMAPHORE_ACQUIRED) return r;
    }
    return SHEMAPHORE_ACQUIRED;
}
