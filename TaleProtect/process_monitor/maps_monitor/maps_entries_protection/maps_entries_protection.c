#include "maps_entries_protection.h"

#include <unstdstring_compat.h>
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/demolisher/threads_kill_recursive.h"
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include <TaleProtect/tools/utils.h>
#include <TaleProtect/gcontext.h>
#include <linux/fcntl.h>
#include <linux/futex.h>

//! /proc/self/maps
static inline __attribute__((always_inline)) unsigned char *decode_proc_self_maps__15(void) {
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
    __hc_tmpB = 0x8C48B424DCBFF51EULL;
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

//! .so
static inline __attribute__((always_inline)) unsigned char *decode_so__3(void) {
    size_t __hc_len = 4u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x0000001BDCCB8000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = 0x0;
    return buf;
}

//! frida
static inline __attribute__((always_inline)) unsigned char *decode_frida__5(void) {
    size_t __hc_len = 6u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x0018591A5C998000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = 0x0;
    return buf;
}

//! jdwp
static inline __attribute__((always_inline)) unsigned char *decode_jdwp__4(void) {
    size_t __hc_len = 5u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x00001C1DD91A8000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = 0x0;
    return buf;
}

void maps_monitor_maps_entries_main_thread_satisfier_once(void) {
    putex_expected_signal(
            &TaleProtectContext.process_monitor.maps_monitor.maps_entries_protection.putex,
            98
    );

    LOGI("(ProcessMonitor::MapsMonitor::MapEntriesProtection::MainThreadSatisfier) main thread has been satisfied.");
}

void *maps_monitor_maps_entries_loop(void *ctx_arg) {
    i8 *decoded_proc_self_maps = (i8 *) decode_proc_self_maps__15();
    int fd = secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, decoded_proc_self_maps, O_RDONLY);
    if (fd < 0) {
        LOGW("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Loop) initial open /proc/self/maps failed: %s",
             strerror(errno));
        secure_memory_wipe(decoded_proc_self_maps, 15, SECURE_MEMORY_WIPE_FREE);
        return NULL;
    }
    secure_memory_wipe(decoded_proc_self_maps, 15, SECURE_MEMORY_WIPE_FREE);

    utils_buffered_fgets_ctx_t fgets_ctx = {.fd = fd};
    maps_monitor_maps_entries_loop_ctx_t *ctx = (maps_monitor_maps_entries_loop_ctx_t *) ctx_arg;

    i8 *decoded_so = (i8 *) decode_so__3();
    i8 *decoded_jdwp = (i8 *) decode_jdwp__4();
    i8 *decoded_frida = (i8 *) decode_frida__5();

    char line[4096];

    /* This is O(scary), but seems quick enough in practice. */
    while (1) {
//        unstdmemory_compat_memset(&ctx, 0, sizeof(ctx));
        ctx->result = 0;

        if (secure_syscall_v2M(SYS_lseek, 3, fd, 0, SEEK_SET) < 0) {
            LOGW("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Loop) lseek failed: %s",
                 strerror(errno));
            utils_msleep(812);
            continue;
        }

        fgets_ctx.start = 0;
        fgets_ctx.end = 0;

        while (utils_buffered_fgets(&fgets_ctx, line, sizeof(line))) {
            size_t ln = unstdstring_compat_strlen8(line);
            if (ln && line[ln - 1] == '\n') {
                line[ln - 1] = 0;
            }

            // LOGD("> maps: %s", line);

            // todo: add frida check back before release
            if (unstdstring_compat_strstr(line, decoded_so)
                && (unstdstring_compat_strcasestr(line, decoded_jdwp)
                    || unstdstring_compat_strcasestr(line, decoded_frida)
                    || utils_is_path_suspicious(line))) {
                LOGE("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Loop) suspicious map entry: %s",
                     line
                );

                pthread_t early_tickles;
                if (pthread_createM(
                        &early_tickles, NULL,
                        (void *(*)(void *)) demolisher_thread_kills_recursive,
                        NULL)) {
                    demolisher_smash();
                }

                if (pthread_detachM(early_tickles)) {
                    demolisher_smash();
                }

                ctx->result = 1;
                break;
            }
        }

        if (ctx->result) {
            LOGE("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Loop) suspicious mapping detected, signaling putex...");
            putex_signal(&ctx->putex);
            break;
        }

        if (pthread_onceM(
                &ctx->main_thread_satisfier_once,
                maps_monitor_maps_entries_main_thread_satisfier_once)) {
            demolisher_smash();
        }

        utils_msleep(338);
//        utils_msleep(38);
    }

    secure_memory_wipe(decoded_so, 3, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_jdwp, 4, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_frida, 5, SECURE_MEMORY_WIPE_FREE);
    secure_syscall_v2M(SYS_close, 1, fd);
    return ctx;
}


void *maps_monitor_maps_entries_runner_thread(void *arg) {
    (void) arg;

    maps_monitor_maps_entries_loop_ctx_t loop_ctx = {0};
    loop_ctx.main_thread_satisfier_once = PTHREAD_ONCE_INIT;
    putex_init(&loop_ctx.putex, 0);

    pthread_t loop_thread;
    if (pthread_createM(&loop_thread, NULL, maps_monitor_maps_entries_loop, &loop_ctx)) {
        demolisher_stackoverflower();
    }

    if (pthread_detachM(loop_thread)) {
        demolisher_stackoverflower();
    }

    putex_wait(&loop_ctx.putex);

    LOGE("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Runner) suspicious mapping detected! self destructing...");

    demolisher_thread_kills_recursive();
    demolisher_thread_kills_recursive();

    for (int i = 0; i < 9; ++i) {
        i += 2;
        LOGE("(ProcessMonitor::MapsMonitor::MapEntriesProtection::Runner) 0xdeadd00dbadb00b");
        demolisher_smash();
        demolisher_stackoverflower();
    }

    return NULL;
}

void maps_monitor_maps_entries_protection(void) {
    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, maps_monitor_maps_entries_runner_thread, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }

    if (pthread_detachM(runner_tid)) {
        demolisher_smash();
    }
}
