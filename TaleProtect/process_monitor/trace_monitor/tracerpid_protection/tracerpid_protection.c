#include "tracerpid_protection.h"

#include <unstdstring_compat.h>
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/demolisher/threads_kill_recursive.h"
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include <TaleProtect/tools/utils.h>
#include <TaleProtect/gcontext.h>
#include <linux/fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define STATUS_BUF_SZ 1024

//! /proc/self/status
static inline __attribute__((always_inline)) unsigned char *decode_proc_self_status__17(void) {
    size_t __hc_len = 18u;
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
    __hc_tmpB = 0xF94FA531E2BFF51EULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[15] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 2 */
    __hc_tmpA = 0xBB84AE0F5C02CD56ULL;
    __hc_tmpA = ((__hc_tmpA - 0x0000000029EDCF0CULL) + 0x00000000A4B8F69CULL);
    __hc_tmpB = 0x1CC0000063A2F5CEULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = 0x0;
    return buf;
}

//! TracerPid:
static inline __attribute__((always_inline)) unsigned char *decode_tracer_pid__10(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x1C9958D85C951A54ULL;
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
    __hc_tmpB = 0x8C3BC4456FD52916ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

void tracerpid_protection_main_thread_satisfier_once(void) {
//    putex_expected_signal(&TaleProtectContext.process_monitor.tracerpid_protection.putex, 42);
    LOGI("(ProcessMonitor::TracerPidProtection::MainThreadSatisfier) main thread has been satisfied.");
}

void *tracerpid_protection_loop(void *ctx_arg) {
    if (!ctx_arg) { return NULL; }

    tracerpid_protection_loop_ctx_t *ctx = (tracerpid_protection_loop_ctx_t *) ctx_arg;

    i8 *decoded_proc_self_status = (i8 *) decode_proc_self_status__17();
    int fd = (int) secure_syscall_v2M(
            SYS_openat, 4,
            AT_FDCWD,
            decoded_proc_self_status,
            O_RDONLY,
            0
    );

    if (fd < 0) {
        LOGW("(ProcessMonitor::TraceMonitor::TracerPidProtection::Loop) open /proc/self/status failed: %s",
             strerror(errno));
        secure_memory_wipe(decoded_proc_self_status, 17, SECURE_MEMORY_WIPE_FREE);
        return NULL;
    }

    secure_memory_wipe(decoded_proc_self_status, 17, SECURE_MEMORY_WIPE_FREE);

    utils_buffered_fgets_ctx_t fgets_ctx = {.fd = fd, .start = 0, .end = 0};
    char line[256];
    ctx->result = 0;

    i8 *decoded_tracer_pid = (i8 *) decode_tracer_pid__10();

    while (1) {
        if (secure_syscall_v2M(SYS_lseek, 3, fd, 0L, SEEK_SET) < 0) {
            LOGW("(ProcessMonitor::TraceMonitor::TracerPidProtection::Loop) lseek failed: %s",
                 strerror(errno));
            utils_msleep(500);
            continue;
        }

        fgets_ctx.start = 0;
        fgets_ctx.end = 0;

        while (utils_buffered_fgets(&fgets_ctx, line, sizeof(line))) {
            size_t ln = unstdstring_compat_strlen8(line);
            if (ln && line[ln - 1] == '\n') line[ln - 1] = 0;

            char *p = unstdstring_compat_strstr(line, decoded_tracer_pid);
            if (p) {
                p += 10;
                while (*p == ' ' || *p == '\t') { ++p; }
                i32 tracer = 0;
                i32 read = 0;
                while (*p >= '0' && *p <= '9') {
                    tracer = tracer * 10 + (*p - '0');
                    ++p;
                    ++read;
                    if (tracer > 10000000) { break; }
                }

//                LOGD("(ProcessMonitor::TraceMonitor::TracerPidProtection::Loop) TracerPid: %d", tracer);

                if (read && tracer != 0) {
                    LOGE("(ProcessMonitor::TraceMonitor::TracerPidProtection::Loop) detected tracer pid=%d",
                         tracer);
                    ctx->result = 1;

                    pthread_t t;
                    if (pthread_createM(&t,
                                        NULL,
                                        (void *(*)(void *)) demolisher_thread_kills_recursive,
                                        NULL)) {
                        demolisher_smash();
                    } else {
                        if (pthread_detachM(t)) {
                            demolisher_stackoverflower();
                        }
                    }

                    putex_signal(&ctx->putex);
                    break;
                }
            }
        }


//        if (pthread_onceM(&ctx->main_thread_satisfier_once,
//                          tracerpid_protection_main_thread_satisfier_once)) {
//            demolisher_smash();
//        }

        utils_msleep(623);
    }

    secure_memory_wipe(decoded_tracer_pid, 10, SECURE_MEMORY_WIPE_FREE);
    secure_syscall_v2M(SYS_close, 1, fd);
    return ctx;
}


void *tracerpid_protection_runner_thread(void *arg) {
    (void) arg;

    tracerpid_protection_loop_ctx_t *loop_ctx = mallocM(sizeof(tracerpid_protection_loop_ctx_t));
    unstdmemory_compat_memset(loop_ctx, 0, sizeof(tracerpid_protection_loop_ctx_t));

//    loop_ctx->main_thread_satisfier_once = PTHREAD_ONCE_INIT;
    putex_init(&loop_ctx->putex, 0);

    pthread_t loop_thread;
    if (pthread_createM(&loop_thread, NULL, tracerpid_protection_loop, &loop_ctx)) {
        demolisher_stackoverflower();
    }

    if (pthread_detachM(loop_thread)) {
        demolisher_stackoverflower();
    }

    putex_wait(&loop_ctx->putex);

    LOGE("(ProcessMonitor::TraceMonitor::TracerPidProtection::Runner) tracer detected! self destructing...");

    demolisher_thread_kills_recursive();

    for (int i = 0; i < 9; ++i) {
        (void) i;
        LOGE("(ProcessMonitor::TraceMonitor::TracerPidProtection::Runner) 0xdeadd00dbadb00b");
        demolisher_smash();
        demolisher_stackoverflower();
    }

    return NULL;
}


void tracerpid_protection(void) {
    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, tracerpid_protection_runner_thread, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }

    if (pthread_detachM(runner_tid)) {
        demolisher_smash();
    }
}
