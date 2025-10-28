#include <unstdstring_compat.h>
#include "utils.h"

#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

static inline __attribute__((always_inline)) unsigned char *decode__data_app___10(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x4BD85D18590BDC18ULL;
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
    __hc_tmpB = 0x8C3BC4456FD53C22ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

void utils_msleep(sizet ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    secure_syscall_v2M(SYS_nanosleep, 2, &ts, NULL);
//    secure_syscall_liteM(SYS_nanosleep, 2, &ts, NULL);
}

i8 *utils_get_native_libraries_dir(void) {
    static i8 dir_path[512] = {0};

    if (*dir_path) {
        return dir_path;
    }

    Dl_info info;
    if (!dladdrM((void *) utils_get_native_libraries_dir, &info) || !info.dli_fname) {
        return NULL;
    }

    const i8 *src = info.dli_fname;
    s16t i = 0;
    for (; i < 511 && src[i] != '\0'; i++) {
        dir_path[i] = src[i];
    }
    dir_path[i] = 0;

    s16t last_slash = -1;
    for (s16t j = 0; j < i; j++) {
        if (dir_path[j] == '/') last_slash = j;
    }

    if (last_slash >= 0) {
        dir_path[last_slash] = 0;
    }

    return dir_path;
}

//! com.tale.royale
static inline __attribute__((always_inline)) unsigned char *decode_com_tale_royale__15(void) {
    size_t __hc_len = 16u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x185D0B9B5BD8D95BULL;
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
    __hc_tmpB = 0x8C5EA824E8FFE0E0ULL;
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

i8 *utils_get_base_apk_install_dir(void) {
    static char result[512] = {0};

    char *full_path = utils_get_native_libraries_dir();
    if (!full_path || !*full_path) {
        return "";
    }

    LOGI("[BaseApkDir] full path: %s", full_path);

    i8 *decoded__data_app = (i8 *) decode__data_app___10();
    const char *start = unstdstring_compat_strstr(full_path, decoded__data_app);
    secure_memory_wipe(decoded__data_app, 10, SECURE_MEMORY_WIPE_FREE);
    if (!start) {
        return "";
    }

    i8 *decoded_com_tale_stars = (i8 *) decode_com_tale_royale__15();
    const char *pkg_pos = strstr(start, decoded_com_tale_stars);
    if (!pkg_pos) {
        return "";
    }
    secure_memory_wipe(decoded_com_tale_stars, 15, SECURE_MEMORY_WIPE_FREE);

    /* find the next '/' after "com.tale.royale" */
    const char *end = unstdstring_compat_strchr(pkg_pos + 14, '/');
    if (!end) {
        return "";
    }

    sizet len = (sizet) (end - start + 1);
    if (len >= sizeof(result)) {
        len = sizeof(result) - 1;
    }

    unstdmemory_compat_memcpy(result, start, len);
    result[len] = '\0';

    LOGD("[BaseApkDir] extracted dir: %s", result);
    return result;
}


//! /data/local/tmp
static inline __attribute__((always_inline)) unsigned char *decode_data_local_tmp__15(void) {
    sizet __hc_len = 16u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x0BD85D18590BDBDBULL;
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
    __hc_tmpB = 0x8C4BA9319F00EE13ULL;
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

//! /data/local/
static inline __attribute__((always_inline)) unsigned char *decode_data_local__12(void) {
    sizet __hc_len = 13u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x0BD85D18590BDBDBULL;
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
    __hc_tmpB = 0x8C3BC4459F00EE13ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = 0x0;
    return buf;
}

//! /sdcard/
static inline __attribute__((always_inline)) unsigned char *decode_sdcard__8(void) {
    sizet __hc_len = 9u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x1C9858D91CCBCBD9ULL;
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
    buf[8] = 0x0;
    return buf;
}

//! /mnt/
static inline __attribute__((always_inline)) unsigned char *decode_mnt__5(void) {
    sizet __hc_len = 6u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x000BDD1B9B4BC000ULL;
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

//! /tmp/
static inline __attribute__((always_inline)) unsigned char *decode_tmp__5(void) {
    sizet __hc_len = 6u;
    unsigned char *buf = (unsigned char *) malloc(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x000BDC1B5D0BC000ULL;
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

u8t utils_is_path_suspicious(const i8 *path_arg) {
    if (!path_arg) {
        return 0;
    }

    u8t result = 0;
    i8 *decoded_buf = NULL;

    decoded_buf = (i8 *) decode_data_local_tmp__15();
    if (decoded_buf) {
        if (unstdstring_compat_strstr(path_arg, decoded_buf)) {
            result = 1;
        }
        secure_memory_wipe(decoded_buf, 15, SECURE_MEMORY_WIPE_FREE);
        if (result) { return result; }
    }

    decoded_buf = (i8 *) decode_data_local__12();
    if (decoded_buf) {
        if (unstdstring_compat_strstr(path_arg, decoded_buf)) {
            result = 1;
        }
        secure_memory_wipe(decoded_buf, 11, SECURE_MEMORY_WIPE_FREE);
        if (result) { return result; }
    }

    decoded_buf = (i8 *) decode_sdcard__8();
    if (decoded_buf) {
        if (unstdstring_compat_strstr(path_arg, decoded_buf)) {
            result = 1;
        }
        secure_memory_wipe(decoded_buf, 8, SECURE_MEMORY_WIPE_FREE);
        if (result) { return result; }
    }

    decoded_buf = (i8 *) decode_mnt__5();
    if (decoded_buf) {
        if (unstdstring_compat_strstr(path_arg, decoded_buf)) {
            result = 1;
        }
        secure_memory_wipe(decoded_buf, 5, SECURE_MEMORY_WIPE_FREE);
        if (result) { return result; }
    }

    decoded_buf = (i8 *) decode_tmp__5();
    if (decoded_buf) {
        if (unstdstring_compat_strstr(path_arg, decoded_buf)) {
            result = 1;
        }
        secure_memory_wipe(decoded_buf, 5, SECURE_MEMORY_WIPE_FREE);
        if (result) { return result; }
    }

    return result;
}

ssizet _utils_fd_fill(utils_buffered_fgets_ctx_t *fb) {
    ssizet r = secure_syscall_v2M(SYS_read, 3, fb->fd, fb->buf, BUFFERED_FGETS_SIZE);

    if (r > 0) {
        fb->start = 0;
        fb->end = (sizet) r;
    }

    return r;
}

ssizet utils_buffered_fgets(utils_buffered_fgets_ctx_t *ctx_arg, i8 *dst_arg, sizet n_arg) {
    if (!ctx_arg || !dst_arg || n_arg == 0) {
        return -1;
    }

    sizet i = 0;
    while (i < n_arg - 1) {
        if (ctx_arg->start >= ctx_arg->end) {
            ssizet r = _utils_fd_fill(ctx_arg);
            if (r <= 0) {
                if (i == 0) { return r; }
                break;
            }
        }

        char c = ctx_arg->buf[ctx_arg->start++];
        dst_arg[i++] = c;
        if (c == '\n') { break; }
    }

    dst_arg[i] = 0;
    return (i <= (size_t) SSIZE_MAX) ? (ssize_t) i : -2;
}
