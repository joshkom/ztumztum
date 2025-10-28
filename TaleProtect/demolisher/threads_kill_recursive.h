#ifndef TALESTARS_V63_322_THREADS_KILL_RECURSIVE_H
#define TALESTARS_V63_322_THREADS_KILL_RECURSIVE_H

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include "global_common/common.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

//! /proc/self/task
static inline __attribute__((always_inline)) unsigned char *_decode_proc_self_task__15(void) {
    size_t __hc_len = 16u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xCBD8DBDC9C0BD95CULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C50B724E3BFF51EULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[15] = 0x0;
    return buf;
}

static int list_threads(pid_t **out_tids, size_t *out_cnt) {
    i8 *decoded_proc_self_task = (i8 *) _decode_proc_self_task__15();
    DIR *d = opendir(decoded_proc_self_task);
    secure_memory_wipe(decoded_proc_self_task, 15, SECURE_MEMORY_WIPE_FREE);
    if (!d) return -1;
    struct dirent *ent;
    size_t cap = 32, cnt = 0;
    pid_t *tids = mallocM(cap * sizeof(pid_t));
    if (!tids) {
        closedir(d);
        return -1;
    }

    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        char *endptr = NULL;
        long v = strtol(ent->d_name, &endptr, 10);
        if (endptr && *endptr == '\0') {
            if (cnt >= cap) {
                cap *= 2;
                pid_t *tmp = realloc(tids, cap * sizeof(pid_t));
                if (!tmp) {
                    freeM(tids);
                    closedir(d);
                    return -1;
                }
                tids = tmp;
            }
            tids[cnt++] = (pid_t) v;
        }
    }
    closedir(d);
    *out_tids = tids;
    *out_cnt = cnt;
    return 0;
}


FORCE_INLINE void demolisher_thread_kills_recursive(void) {
    pid_t pid = (pid_t) secure_syscallM(SYS_getpid, 0);
    pid_t self_tid = (pid_t) secure_syscallM(SYS_gettid, 0);

    pid_t *tids = NULL;
    size_t cnt = 0;
    if (list_threads(&tids, &cnt) == 0) {
        for (size_t i = 0; i < cnt; ++i) {
            pid_t tid = tids[i];
            LOGI("killing tid[%d]", tid);
            if (tid == self_tid) {
                LOGW("skipped killing self_tid[%d]", tid);
                continue;
            }

            secure_syscallM(SYS_tgkill, 3, pid, tid, SIGTERM);
        }
        freeM(tids);
    }

    utils_msleep(10);

    if (list_threads(&tids, &cnt) == 0) {
        for (sizet i = 0; i < cnt; ++i) {
            pid_t tid = tids[i];
            secure_syscallM(SYS_tgkill, 3, pid, tid, SIGKILL);
        }
        freeM(tids);
    }

    secure_syscallM(SYS_kill, 2, 0, 9);
}


#endif //TALESTARS_V63_322_THREADS_KILL_RECURSIVE_H
