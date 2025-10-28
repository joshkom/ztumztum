#include "putex.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include <linux/futex.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include <bits/stdatomic.h>


ATTR_ALWAYS_INLINE i32
putex_futex_wait(atomic_int *addr, i32 expected, const struct timespec *timeout) {
    return secure_syscall_liteM(SYS_futex, 6, addr, FUTEX_WAIT, expected, timeout, NULL, 0);
}

ATTR_ALWAYS_INLINE i32 putex_futex_wake(atomic_int *addr, i32 n) {
    return secure_syscall_liteM(SYS_futex, 6, addr, FUTEX_WAKE, n, NULL, NULL, 0);
}

void putex_init(putex_t *ptx, atomic_int initial) {
    atomic_store(&ptx->val, initial);
}

void putex_signal(putex_t *ptx_arg) {
    int old = atomic_fetch_add(&ptx_arg->val, 1);
    if (old < 0) {
        putex_futex_wake(&ptx_arg->val, 1);
    }
}

void putex_wait(putex_t *ptx_arg) {
    while (1) {
        int v = atomic_load(&ptx_arg->val);
        if (v > 0) {
            if (atomic_compare_exchange_weak(&ptx_arg->val, &v, v - 1))
                return; /* acquired */
        } else {
            putex_futex_wait(&ptx_arg->val, 0, NULL);
        }
    }
}

putex_result_t putex_wait_timeout(putex_t *ptx_arg, i32 sec_arg, ssizet nsec_arg) {
    struct timespec ts = {.tv_sec = sec_arg, .tv_nsec = nsec_arg};

    while (1) {
        int v = atomic_load(&ptx_arg->val);
        if (v > 0) {
            if (atomic_compare_exchange_weak(&ptx_arg->val, &v, v - 1)) {
                return PUTEX_ACQUIRED; /* acquired */
            }
        } else {
            int s = putex_futex_wait(&ptx_arg->val, 0, &ts);
            if (s == -1) {
                if (errno == ETIMEDOUT) {
                    return PUTEX_TIMEOUT;
                } else if (errno == EAGAIN) {
                    continue;  /* value changed, retry */
                } else if (errno == EINTR) {
                    continue;  /* interrupted, retry */
                } else {
                    LOGE("putex_wait_timeout(%d) ? %s", s, strerror(errno));
                    return PUTEX_FAILURE; /* unk? */
                }
            }
        }
    }
}

//! [putex_expected]
void putex_expected_signal(putex_t *ptx_arg, atomic_int value_arg) {
    atomic_store(&ptx_arg->val, value_arg);
    putex_futex_wake(&ptx_arg->val, 1);
}

void putex_expected_wait(putex_t *ptx_arg, atomic_int expected_value_arg) {
    while (atomic_load(&ptx_arg->val) != expected_value_arg) {
        putex_futex_wait(&ptx_arg->val, expected_value_arg, NULL);
    }
}

putex_result_t putex_expected_wait_timeout(putex_t *ptx_arg, atomic_int expected_value_arg,
                                           i32 sec_arg, ssizet nsec_arg) {
    struct timespec ts = {.tv_sec = sec_arg, .tv_nsec = nsec_arg};

    while (atomic_load(&ptx_arg->val) != expected_value_arg) {
        int val = atomic_load(&ptx_arg->val);
        int s = putex_futex_wait(&ptx_arg->val, val, &ts);
        if (s == -1) {
            if (errno == ETIMEDOUT) {
                return PUTEX_TIMEOUT;
            } else if (errno == EAGAIN) {
                continue;  /* value changed, retry */
            } else if (errno == EINTR) {
                continue;  /* interrupted, retry */
            } else {
                LOGE("putex_expected_wait_timeout_err(%d) ? %s", s, strerror(errno));
                return PUTEX_FAILURE; /* unk? */
            }
        }
    }

    return PUTEX_ACQUIRED; /* acquired */
}
