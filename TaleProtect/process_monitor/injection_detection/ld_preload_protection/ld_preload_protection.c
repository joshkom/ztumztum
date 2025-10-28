#include "ld_preload_protection.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/demolisher/smash.h"

#include <unstdstring_compat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unstdprintf_compat.h>

//! LD_PRELOAD
static inline __attribute__((always_inline)) unsigned char *decode_ld_preload1__10(void) {
    size_t __hc_len = 10u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x11549417D11313D3ULL;
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
    __hc_tmpB = 0x8C3BC4456FD51331ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    return buf;
}

//! LD_PRELOAD=
static inline __attribute__((always_inline)) unsigned char *decode_ld_preload2__11(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x11549417D11313D3ULL;
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
    __hc_tmpB = 0x8C3BC4456FD21331ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    return buf;
}

//! /proc/%d/environ
static inline __attribute__((always_inline)) unsigned char *decode_proc_pid_environ__16(void) {
    size_t __hc_len = 17u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) {
        return NULL;
    }
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x4BD8DBDC9C0BD909ULL;
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
    __hc_tmpB = 0xE254B62CE5FEF1DFULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[15] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    buf[16] = 0x0;
    return buf;
}

//static injdetection_ld_preload_inspection_t check_proc_environ(void) {
//    injdetection_ld_preload_inspection_t r = {0};
//    char buf[4096];
//    ssize_t nread;
//
//    i8 proc_pid_environ[24] = {0};
//
//    i8 *decoded_proc_pid_environ = (i8 *) decode_proc_pid_environ__16();
//
//    LOGI(
//            "(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) decoded: %s | pid: %d",
//            decoded_proc_pid_environ,
//            (pid_t) secure_syscall_v2M(SYS_getpid, 0)
//    );
//
//    unstdprintf_snprintf(
//            proc_pid_environ,
//            24,
//            decoded_proc_pid_environ,
//            (pid_t) secure_syscall_v2M(SYS_getpid, 0)
//    );
//
//    LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection) environ path: %s",
//         proc_pid_environ);
//
//    secure_memory_wipe(
//            (void *) decoded_proc_pid_environ,
//            16,
//            SECURE_MEMORY_WIPE_FREE
//    );
//
//    int dirfd = AT_FDCWD;
//    int fd = openat(dirfd, proc_pid_environ, O_RDONLY);
//    if (fd < 0) {
//        LOGD("openat /proc/self/environ failed: %s", strerror(errno));
//        return r;
//    }
//
//    nread = read(fd, buf, sizeof(buf) - 1);
//    close(fd);
//    if (nread <= 0) {
//        LOGD("read /proc/self/environ failed or empty: %zd", nread);
//        return r;
//    }
//
//    buf[nread] = 0;
//
//    size_t off = 0;
//    while (off < (size_t) nread) {
//        char *entry = buf + off;
//        size_t elen = strnlen(entry, nread - off);
//        if (elen == 0) {
//            off += 1;
//            continue;
//        }
//        const char prefix[] = "LD_PRELOAD=";
//        size_t prelen = sizeof(prefix) - 1;
//        if (elen > prelen && strncmp(entry, prefix, prelen) == 0) {
//            r.result = 1;
//            size_t copylen = elen - prelen;
//            if (copylen > LD_PRELOAD_MAX) {
//                copylen = LD_PRELOAD_MAX;
//            }
//
//            memcpy(r.ld_preload_value, entry + prelen, copylen);
//            r.ld_preload_value[copylen] = 0;
//
//            LOGI("/proc/self/environ: found LD_PRELOAD='%s'", r.ld_preload_value);
//            break;
//        } else {
//            LOGD("env_entry[%zd]: %s", nread, entry + prelen);
//        }
//        off += elen + 1;
//    }
//
//    return r;
//}

injdetection_ld_preload_inspection_t injdetection_ld_preload_inspection(void) {
    injdetection_ld_preload_inspection_t r = {0};

    const i8 *decoded_ld_preload1 = (i8 *) decode_ld_preload1__10();

    const i8 *ld = getenvM(decoded_ld_preload1);
    if (ld && ld[0]) {
        LOGI("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) getenv: LD_PRELOAD='%s'",
             ld);
        r.result = 1;
        unstdmemory_compat_memcpy(
                r.ld_preload_value,
                ld,
                (
                        unstdstring_compat_strlen8(ld) > 512
                        ? 512
                        : unstdstring_compat_strlen8(ld)
                )
        );

        secure_memory_wipe(
                (void *) decoded_ld_preload1,
                10,
                SECURE_MEMORY_WIPE_FREE
        );

        return r;
    } else {
        r.result = 0;
        LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) getenv: LD_PRELOAD not set");
    }

    secure_memory_wipe(
            (void *) decoded_ld_preload1,
            10,
            SECURE_MEMORY_WIPE_FREE
    );

    if (environ) {
        LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) begin environ dump");
        u8t *ld_preload2 = decode_ld_preload2__11();
        for (char **e = environ; *e; ++e) {
            LOGD("ENV: %s", *e);
            if (unstdmemory_compat_memcmp(*e, ld_preload2, 11) == 0) {
                r.result = 1;
                unstdmemory_compat_memcpy(
                        r.ld_preload_value,
                        *e,
                        (
                                unstdstring_compat_strlen8(*e) > 512
                                ? 512
                                : unstdstring_compat_strlen8(*e)
                        )
                );
                break;
            }
        }
        secure_memory_wipe(
                (void *) ld_preload2,
                11,
                SECURE_MEMORY_WIPE_FREE
        );
        LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) end environ dump");
        if (r.result == 1) {
            return r;
        }
    } else {
        r.result = 0;
        LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) environ not available");
    }

    //! check /proc/getpid/environ (this thing is readonly...)
//    injdetection_ld_preload_inspection_t r2 = check_proc_environ();
//    if (r2.result) {
//        r.result = 1;
//        memcpy(r.ld_preload_value, r2.ld_preload_value, LD_PRELOAD_MAX);
//        LOGI("LD_PRELOAD/env indicates possible injection (%s)", r.ld_preload_value);
//        return r;
//    } else {
//        LOGD("no LD_PRELOAD entries in /proc/self/environ");
//    }

    if (!r.result) {
        LOGD("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) no LD_PRELOAD/env suspicious entries");
    } else {
        LOGE("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Inspection) LD_PRELOAD/env indicates possible injection (%s)",
             r.ld_preload_value);
    }

    return r;
}

void injdetection_ld_preload_mitigation(injdetection_ld_preload_inspection_t buffer_arg) {
    if (buffer_arg.result != 0 || buffer_arg.ld_preload_value[0]) {
        LOGE("(ProcessMonitor::InjectionDetection::LD_PRELOADProtection::Mitigation) burning smell...");
        demolisher_stackoverflower();
        demolisher_smash();
    }
}

void injdetection_ld_preload_protection(void) {
    injdetection_ld_preload_mitigation(injdetection_ld_preload_inspection());
}
