//
// Created by johan on 12.10.25.
//

#include <unstdbool.h>
#include <unstdprintf_compat.h>

#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <unstdstring_compat.h>

#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/nicetits/nicetits.h"
#include "TaleProtect/talecrypt/compat/talecrypt.h"
#include "TaleProtect/tools/dawnloader/dawnloader.h"
#include "TaleProtect/metas/jprovider.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

#ifndef SHA256_DIGEST_LENGTH
#define SHA256_DIGEST_LENGTH 32
#endif

//! %sbase.apk
static inline __attribute__((always_inline)) unsigned char *decode_fmt_base_apk__10(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x995CD8589CC9584BULL;
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
    __hc_tmpB = 0x8C3BC4456FD4F822ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_url_nicetits__34(void) {
    size_t __hc_len = 35u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xCE9CDC1D1D1A0BCBULL;
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
    __hc_tmpB = 0xA256AB2ED803FC1CULL;
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
    __hc_tmpB = 0x59DA4B98FEF8E8D4ULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[18] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[19] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[20] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[21] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[22] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[23] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 3 */
    __hc_tmpA = 0xEE750934BBDD9D53ULL;
    __hc_tmpA = (((__hc_tmpA + 0x000000009BFFEE58ULL) + 0x000000005247DC16ULL) ^
                 0x000000006F6967EFULL);
    __hc_tmpB = 0x5D1958DA02C79790ULL;
    __hc_tmpB = ((((__hc_tmpB + 0x0000000058C4444AULL) >> 6) |
                  ((__hc_tmpB + 0x0000000058C4444AULL) << 58)) & 0xFFFFFFFFFFFFFFFFULL);
    buf[24] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[25] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[26] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[27] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[28] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[29] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[30] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[31] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 4 */
    __hc_tmpA = 0x905E5F3E75758FF6ULL;
    __hc_tmpA = ((((((__hc_tmpA * 0x00000000E4D4C85FULL) - 0x000000009C867C18ULL) >> 19) |
                   (((__hc_tmpA * 0x00000000E4D4C85FULL) - 0x000000009C867C18ULL) << 45)) &
                  0xFFFFFFFFFFFFFFFFULL) - 0x000000009E5D19BDULL);
    __hc_tmpB = 0x4A2032CDBA28A659ULL;
    __hc_tmpB = (((((__hc_tmpB >> 59) | (__hc_tmpB << 5)) & 0xFFFFFFFFFFFFFFFFULL) +
                  0x00000000A03E6ECFULL) ^ __hc_tmpA);
    buf[32] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[33] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[34] = 0x0;
    return buf;
}
static inline __attribute__((always_inline)) unsigned char* decode_https___joshkom_github_io_nicedeck__34(void){
    size_t __hc_len = 35u;
    unsigned char* buf = (unsigned char*)mallocM(__hc_len);
    if(!buf) return NULL;
    volatile unsigned long long __hc_tmpA,__hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL; __hc_tmpA = (((( (__hc_tmpA * 0x000000009D5E4B49ULL) >> 38 ) | ( (__hc_tmpA * 0x000000009D5E4B49ULL) << 26 )) & 0xFFFFFFFFFFFFFFFFULL) ^ 0x0000000036970A05ULL);
    __hc_tmpB = 0xCE9CDC1D1D1A0BCBULL; __hc_tmpB = ((( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) << 36 ) | ( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) >> 28 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL; __hc_tmpA = (((( (__hc_tmpA + 0x0000000073A5E55CULL) << 26 ) | ( (__hc_tmpA + 0x0000000073A5E55CULL) >> 38 )) & 0xFFFFFFFFFFFFFFFFULL) * 0x000000008DA8AF8BULL);
    __hc_tmpB = 0xA256AB2ED803FC1CULL; __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[15] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 2 */
    __hc_tmpA = 0xBB84AE0F5C02CD56ULL; __hc_tmpA = ((__hc_tmpA - 0x0000000029EDCF0CULL) + 0x00000000A4B8F69CULL);
    __hc_tmpB = 0x59DA4B98FEF8E8D4ULL; __hc_tmpB = ((( ((( (__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56 ) | ( (__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8 )) & 0xFFFFFFFFFFFFFFFFULL) << 2 ) | ( ((( (__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56 ) | ( (__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8 )) & 0xFFFFFFFFFFFFFFFFULL) >> 62 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[18] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[19] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[20] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[21] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[22] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[23] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 3 */
    __hc_tmpA = 0xEE750934BBDD9D53ULL; __hc_tmpA = (((__hc_tmpA + 0x000000009BFFEE58ULL) + 0x000000005247DC16ULL) ^ 0x000000006F6967EFULL);
    __hc_tmpB = 0x591958DA02C7978FULL; __hc_tmpB = ((( (__hc_tmpB + 0x0000000058C4444AULL) >> 6 ) | ( (__hc_tmpB + 0x0000000058C4444AULL) << 58 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[24] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[25] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[26] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[27] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[28] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[29] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[30] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[31] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 4 */
    __hc_tmpA = 0x905E5F3E75758FF6ULL; __hc_tmpA = (((( ((__hc_tmpA * 0x00000000E4D4C85FULL) - 0x000000009C867C18ULL) >> 19 ) | ( ((__hc_tmpA * 0x00000000E4D4C85FULL) - 0x000000009C867C18ULL) << 45 )) & 0xFFFFFFFFFFFFFFFFULL) - 0x000000009E5D19BDULL);
    __hc_tmpB = 0x022032CDBA28A599ULL; __hc_tmpB = ((((( __hc_tmpB >> 59 ) | ( __hc_tmpB << 5 )) & 0xFFFFFFFFFFFFFFFFULL) + 0x00000000A03E6ECFULL) ^ __hc_tmpA);
    buf[32] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[33] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[34] = 0x0;
    return buf;
}

static inline u64t readLE64(const u8t *buf) {
    return ((u64t) buf[0]) |
           ((u64t) buf[1] << 8) |
           ((u64t) buf[2] << 16) |
           ((u64t) buf[3] << 24) |
           ((u64t) buf[4] << 32) |
           ((u64t) buf[5] << 40) |
           ((u64t) buf[6] << 48) |
           ((u64t) buf[7] << 56);
}

static i32 consttime_eq(const u8t *a, const u8t *b, sizet n) {
    u32t diff = 0;
    for (sizet i = 0; i < n; ++i) { diff |= (u32t) (a[i] ^ b[i]); }
    return diff == 0;
}

bool app_integrity_verify_apk_signature_block_sha256_evp(const i8 *filepath_arg,
                                                         u64t magic_offset_arg,
                                                         const u8t expected_sig_arg[32]) {
    if (!filepath_arg || !expected_sig_arg) { return false; }

    i32 fd = (int) secure_syscall_v2M(
            SYS_openat, 4,
            AT_FDCWD,
            (long) filepath_arg,
            (long) O_RDONLY,
            0L
    );

    if (fd < 0) {
        return false;
    }

    off_t fileSize = (off_t) secure_syscall_v2M(SYS_lseek, 3, fd, 0L, SEEK_END);
    if (fileSize < 0) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    if (magic_offset_arg < 8 || magic_offset_arg >= (u64t) fileSize) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    u64t footerSizeOffset = magic_offset_arg - 8;
    u8t footerSizeBuf[8];
    ssizet r = (ssizet) secure_syscall_v2M(
            SYS_pread64, 4,
            fd,
            (long) footerSizeBuf,
            (long) 8,
            (long) footerSizeOffset
    );
    if (r != 8) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }
    u64t footerSize = readLE64(footerSizeBuf);

    if (footerSize > magic_offset_arg) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }
    u64t headerSizeOffset = magic_offset_arg - footerSize + 8;

    if (headerSizeOffset + 8 > (u64t) fileSize) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    u8t headerSizeBuf[8];
    r = (ssizet) secure_syscall_v2M(
            SYS_pread64, 4,
            fd,
            (long) headerSizeBuf,
            (long) 8,
            (long) headerSizeOffset
    );
    if (r != 8) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }
    u64t headerSize = readLE64(headerSizeBuf);

    if (headerSize != footerSize) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    u64t blockSize = footerSize + 8;
    if (headerSizeOffset + blockSize > (u64t) fileSize) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    u8t *block = (u8t *) mallocM((sizet) blockSize);
    if (!block) {
        secure_syscall_v2M(SYS_close, 1, fd);
        return false;
    }

    r = (ssizet) secure_syscall_v2M(
            SYS_pread64, 4,
            fd,
            (long) block,
            (long) blockSize,
            (long) headerSizeOffset
    );

    if (r != (ssizet) blockSize) {
        secure_syscall_v2M(SYS_close, 1, fd);
        freeM(block);
        return false;
    }

    secure_syscall_v2M(SYS_close, 1, fd);

    bool ok = false;
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        freeM(block);
        return false;
    }

    const EVP_MD *md = EVP_sha256();
    if (!md) {
        EVP_MD_CTX_free(mdctx);
        freeM(block);
        return false;
    }

    if (EVP_DigestInit_ex(mdctx, md, NULL) != 1) goto cleanup;
    if (EVP_DigestUpdate(mdctx, block, (sizet) blockSize) != 1) { goto cleanup; }

    u8t digest[SHA256_DIGEST_LENGTH];
    u32t digest_len = 0;
    if (EVP_DigestFinal_ex(mdctx, digest, &digest_len) != 1) { goto cleanup; }
    if (digest_len != SHA256_DIGEST_LENGTH) { goto cleanup; }

    if (consttime_eq(digest, expected_sig_arg, SHA256_DIGEST_LENGTH)) { ok = true; }

    cleanup:
    OPENSSL_cleanse(block, (sizet) blockSize);
    freeM(block);

    EVP_MD_CTX_free(mdctx);
    return ok;
}

void *app_integrity_inspection(void *arg) {
    (void) arg;
    LOGI("(IntegrityMonitor::AppIntegrity::Inspection) base dir: %s",
         utils_get_base_apk_install_dir()
    );

//    app_integrity_verify_apk_signature_block_sha256_evp()

    static uint8_t KEY[32] = {
            0x0a ^ NICETITS_CONF_XOR_KEY, 0xba ^ NICETITS_CONF_XOR_KEY,
            0x01 ^ NICETITS_CONF_XOR_KEY, 0xf2 ^ NICETITS_CONF_XOR_KEY,
            0x91 ^ NICETITS_CONF_XOR_KEY, 0x19 ^ NICETITS_CONF_XOR_KEY,
            0xf0 ^ NICETITS_CONF_XOR_KEY, 0xc1 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0xbb ^ NICETITS_CONF_XOR_KEY,
            0x3f ^ NICETITS_CONF_XOR_KEY, 0xaf ^ NICETITS_CONF_XOR_KEY,
            0x72 ^ NICETITS_CONF_XOR_KEY, 0x39 ^ NICETITS_CONF_XOR_KEY,
            0xff ^ NICETITS_CONF_XOR_KEY, 0xda ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0xb1 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0xd4 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY,
            0x3f ^ NICETITS_CONF_XOR_KEY, 0xc7 ^ NICETITS_CONF_XOR_KEY,
            0x12 ^ NICETITS_CONF_XOR_KEY, 0x60 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY
    };

    static uint8_t IV[16] = {
            0xff ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY,
            0x00 ^ NICETITS_CONF_XOR_KEY, 0x98 ^ NICETITS_CONF_XOR_KEY,
            0xa8 ^ NICETITS_CONF_XOR_KEY, 0xe5 ^ NICETITS_CONF_XOR_KEY,
            0xf2 ^ NICETITS_CONF_XOR_KEY, 0x4e ^ NICETITS_CONF_XOR_KEY,
            0x4a ^ NICETITS_CONF_XOR_KEY, 0xc1 ^ NICETITS_CONF_XOR_KEY,
            0xa0 ^ NICETITS_CONF_XOR_KEY, 0x1c ^ NICETITS_CONF_XOR_KEY,
            0xff ^ NICETITS_CONF_XOR_KEY, 0x00 ^ NICETITS_CONF_XOR_KEY
    };

    nicetits_xor_decrypt_buffer(KEY, sizeof(KEY));
    nicetits_xor_decrypt_buffer(IV, sizeof(IV));

//    i8 *decoded_url_nicetits_optimised = (i8 *) decode_https___joshkom_github_io_nicetits_lowend__41();
    i8 *decoded_url_nicetits_optimised = (i8 *) decode_https___joshkom_github_io_nicedeck__34();

    dawnloader_result_t dawnloader_result = dawnloader_auto(
            NULL,
            meta_jprovider->vm,
            decoded_url_nicetits_optimised
    );

    secure_memory_wipe(
            decoded_url_nicetits_optimised,
//            41,
            34,
            SECURE_MEMORY_WIPE_FREE
    );

    // high-end devices apk
//    i8 *decoded_url_nicetits_optimised = (i8 *) decode_url_nicetits__34();
//
//    dawnloader_result_t dawnloader_result = dawnloader_auto(
//            NULL,
//            meta_jprovider->vm,
//            decoded_url_nicetits_optimised
//    );
//
//    secure_memory_wipe(
//            decoded_url_nicetits_optimised,
//            34,
//            SECURE_MEMORY_WIPE_FREE
//    );

    LOGD("(IntegrityMonitor::AppIntegrity::Inspection) Download result: %d",
         dawnloader_result.success
    );
    LOGD("(IntegrityMonitor::AppIntegrity::Inspection) Download size: %zu",
         dawnloader_result.size
    );

    if (!dawnloader_result.success) {
        demolisher_smash();
    }

    AES256Encryptor acryptor;
    aes256_init(
            &acryptor,
            KEY,
            sizeof(KEY)
    );

    if (!aes256_decrypt_and_deserialize_from_buffer(
            &acryptor,
            ((ubytes) {
                    .data = dawnloader_result.data,
                    .length = dawnloader_result.size,
                    .capacity = dawnloader_result.size
            }),
            ((ubytes) {
                    .data = IV,
                    .length = sizeof(IV),
                    .capacity = sizeof(IV)
            }),
            &nicetits,
            sizeof(nicetits_t)
    )) {
        LOGE("(IntegrityMonitor::AppIntegrity::Inspection) failed to decrypt.");
        secure_memory_wipe(KEY, sizeof(KEY), SECURE_MEMORY_WIPE_KEEP);
        secure_memory_wipe(IV, sizeof(IV), SECURE_MEMORY_WIPE_KEEP);
        demolisher_smash();
    }

    LOGD("(IntegrityMonitor::AppIntegrity::Inspection) apksigv2->offset(%lu) | (expected) apksigv2->sha256:",
         nicetits.apksigv2.offset
    );
    hexdump(nicetits.apksigv2.hashset.sha256, SHA256_DIGEST_LENGTH);

    const i8 *install_dir = utils_get_base_apk_install_dir();
    if (!install_dir || !*install_dir) {
        LOGE("Failed to get install dir");
        return NULL;
    }

    i8 *decoded_fmt_base_apk = (i8 *) decode_fmt_base_apk__10();
    i8 apk_path[512];
    unstdprintf_snprintf(apk_path, sizeof(apk_path), decoded_fmt_base_apk, install_dir);
    secure_memory_wipe(
            decoded_fmt_base_apk,
            10,
            SECURE_MEMORY_WIPE_FREE
    );


    LOGI("(IntegrityMonitor::AppIntegrity::Inspection) APK Path: %s", apk_path);

    bool ok = app_integrity_verify_apk_signature_block_sha256_evp(
            apk_path,
            nicetits.apksigv2.offset,
            nicetits.apksigv2.hashset.sha256
    );

    if (ok) {
        LOGW("(IntegrityMonitor::AppIntegrity::Inspection) Signature block verification: [PASSED]");
    } else {
        LOGE("(IntegrityMonitor::AppIntegrity::Inspection) Signature block verification: [FAILED]");
        //!! todo: uncomment before release
        demolisher_stackoverflower();
        demolisher_smash();
    }

    secure_memory_wipe(
            apk_path,
            unstdstring_compat_strlen8(apk_path),
            SECURE_MEMORY_WIPE_KEEP
    );

    secure_memory_wipe(KEY, sizeof(KEY), SECURE_MEMORY_WIPE_KEEP);
    secure_memory_wipe(IV, sizeof(IV), SECURE_MEMORY_WIPE_KEEP);

    secure_memory_wipe(
            &acryptor,
            sizeof(AES256Encryptor),
            SECURE_MEMORY_WIPE_KEEP
    );

    secure_memory_wipe(
            &dawnloader_result,
            sizeof(dawnloader_result_t),
            SECURE_MEMORY_WIPE_KEEP
    );

    return NULL;
}

void app_integrity_protection(void) {
    pthread_t pth;
    if (pthread_createM(&pth, NULL, app_integrity_inspection, NULL)) {
        demolisher_stackoverflower();
    }
    if (pthread_detachM(pth)) {
        demolisher_stackoverflower();
    }
}
