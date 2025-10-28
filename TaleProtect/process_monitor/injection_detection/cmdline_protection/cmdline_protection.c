#include <string.h>
#include <errno.h>
#include "cmdline_protection.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include "TaleProtect/tools/utils.h"

#include <unstdstring_compat.h>
#include <asm-generic/fcntl.h>
#include <linux/fcntl.h>

//! rb
static inline __attribute__((always_inline)) unsigned char *decode_rb__2(void) {
    size_t __hc_len = 3u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x00000000189C8000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = 0x0;
    return buf;
}

//! /proc/self/cmdline
static inline __attribute__((always_inline)) unsigned char *decode_proc_self_cmdline__18(void) {
    size_t __hc_len = 19u;
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
    __hc_tmpB = 0xE557A028D2BFF51EULL;
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
    __hc_tmpB = 0x5B80000063A2F5D7ULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[18] = 0x0;
    return buf;
}

//! logwrapper
static inline __attribute__((always_inline)) unsigned char *decode_logwrapper__10(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x185C9DD9DBDB1C1CULL;
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
    __hc_tmpB = 0x8C3BC4456FD4E115ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

injdetection_cmdline_inspection_t injdetection_cmdline_inspection(void) {
    injdetection_cmdline_inspection_t r = {0};

    const i8 *decoded_proc_self_cmdline = (i8 *) decode_proc_self_cmdline__18();
    const i8 *decoded_rb = (i8 *) decode_rb__2();

    ssize_t len;
    i32 f = secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, decoded_proc_self_cmdline, O_RDONLY);
    if (!f) {
        LOGW("(ProcessMonitor::InjectionDetection::CmdlineProtection::Inspection) fopen(/proc/self/cmdline) failed: %s",
             strerror(errno));
        r.result = 0;

        secure_memory_wipe(
                (void *) decoded_proc_self_cmdline,
                18,
                SECURE_MEMORY_WIPE_FREE
        );

        secure_memory_wipe(
                (void *) decoded_rb,
                2,
                SECURE_MEMORY_WIPE_FREE
        );

        return r;
    }

    secure_memory_wipe(
            (void *) decoded_proc_self_cmdline,
            18,
            SECURE_MEMORY_WIPE_FREE
    );

    secure_memory_wipe(
            (void *) decoded_rb,
            2,
            SECURE_MEMORY_WIPE_FREE
    );

    len = secure_syscall_v2M(
            SYS_read,
            3,
            f,
            r.cmdline,
            (sizeof(r.cmdline) - 1)
    );

    secure_syscall_v2M(SYS_close, 1, f);
    if (len <= 0) {
        LOGD("(ProcessMonitor::InjectionDetection::CmdlineProtection::Inspection) empty cmdline");
        return r;
    }
    r.cmdline[len] = 0;

    LOGD("(ProcessMonitor::InjectionDetection::CmdlineProtection::Inspection) cmdline: \"%s\"",
         r.cmdline);

    i8 *decoded_logwrapper = (i8 *) decode_logwrapper__10();
    if (unstdstring_compat_strstr(r.cmdline, decoded_logwrapper)
        || utils_is_path_suspicious(r.cmdline)) {
        LOGE("(ProcessMonitor::InjectionDetection::CmdlineProtection::Inspection) suspicious cmdline: %s",
             r.cmdline);
        r.result = 1;
    }
    secure_memory_wipe(decoded_logwrapper, 10, SECURE_MEMORY_WIPE_FREE);

    //! [check parent cmdline]
//    pid_t ppid = secure_syscall_v2M(SYS_getppid, 0);
//    if (ppid > 1) {
//        char ppath[64];
//        snprintf(ppath, sizeof(ppath), "/proc/%d/cmdline", (int) ppid);
//        FILE *pf = fopen(ppath, "rb");
//        if (pf) {
//            len = fread(r.parent_cmdline, 1, sizeof(r.parent_cmdline) - 1, pf);
//            fclose(pf);
//            if (len > 0) {
//                r.parent_cmdline[len] = '\0';
//                LOGD("parent(%d) cmdline: %s", ppid, r.parent_cmdline);
//                if (unstdstring_compat_strstr(r.parent_cmdline, "logwrapper") ||
//                        utils_is_path_suspicious(r.parent_cmdline)) {
//                    LOGI("injdetection_cmdline_inspection: parent cmdline suspicious: %s",
//                         r.parent_cmdline);
//                    r.result = 1;
//                }
//            }
//        } else {
//            LOGD("couldn't open parent cmdline %s: %s", ppath, strerror(errno));
//        }
//    }

    return r;
}

void injdetection_cmdline_mitigation(injdetection_cmdline_inspection_t buffer_arg) {
    if (buffer_arg.result != 0) {
        LOGE("(ProcessMonitor::InjectionDetection::CmdlineProtection::Mitigation) Suspicious cmdline detected...");
        demolisher_stackoverflower();
        demolisher_smash();
    }
}

void injdetection_cmdline_protection(void) {
    injdetection_cmdline_mitigation(injdetection_cmdline_inspection());
}
