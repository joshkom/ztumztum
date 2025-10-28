#include "net_monitor_protection.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/demolisher/threads_kill_recursive.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <android/log.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <unstdio_compat.h>
#include <unstdstring_compat.h>
#include <unstdprintf_compat.h>

//! frida
static inline __attribute__((always_inline)) u8t *decode_frida__5(void) {
    sizet __hc_len = 6u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x0018591A5C998000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = 0x0;
    return buf;
}

//! frida-gadget
static inline __attribute__((always_inline)) u8t *decode_frida_gadget__12(void) {
    sizet __hc_len = 13u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xCB58591A5C999859ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC445E3F9F416ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = 0x0;
    return buf;
}

//! frida-server
static inline __attribute__((always_inline)) u8t *decode_frida_server__12(void) {
    sizet __hc_len = 13u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xCB58591A5C99995CULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC445E1F9E524ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = 0x0;
    return buf;
}

//! frida-agent
static inline __attribute__((always_inline)) u8t *decode_frida_agent__11(void) {
    sizet __hc_len = 12u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x57D8591A5C9999D8ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4457008FD15ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = 0x0;
    return buf;
}

//! frida-pipe
static inline __attribute__((always_inline)) u8t *decode_frida_pipe__10(void) {
    sizet __hc_len = 11u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x17D8591A5C999A5CULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD4F222ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

//! interceptor
static inline __attribute__((always_inline)) u8t *decode_interceptor__11(void) {
    sizet __hc_len = 12u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x58DC995D1B9A5C19ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4457002FC26ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = 0x0;
    return buf;
}

//! substrate
static inline __attribute__((always_inline)) u8t *decode_substrate__9(void) {
    sizet __hc_len = 10u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x5C9D1CD89D5CDD18ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F15ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

//! rpcgadget
static inline __attribute__((always_inline)) u8t *decode_rpcgadget__9(void) {
    sizet __hc_len = 10u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xD91859D8DC1C9959ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (u8t) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (u8t) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F26ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

//! inject
static inline __attribute__((always_inline)) u8t *decode_inject__6(void) {
    sizet __hc_len = 7u;
    u8t *buf = (u8t *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile u64lt __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x1D18D95A9B9A4000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (u8t) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode__proc__d_net_tcp6__17(void) {
    size_t __hc_len = 18u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
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
    __hc_tmpB = 0xFC58B06AE3F9FCDFULL;
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
    __hc_tmpB = 0x0D80000063A2F5CEULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode__proc__d_net_tcp__16(void) {
    size_t __hc_len = 17u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
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
    __hc_tmpB = 0xFC58B06AE3F9FCDFULL;
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

static inline __attribute__((always_inline)) unsigned char *decode__proc__d_net_unix__17(void) {
    size_t __hc_len = 18u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
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
    __hc_tmpB = 0xE555B16AE3F9FCDFULL;
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
    __hc_tmpB = 0x1E00000063A2F5CEULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode__proc_net_tcp6__14(void) {
    size_t __hc_len = 15u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x8BD8DBDC9C0BD95BULL;
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
    __hc_tmpB = 0x8C3BF235D3093C26ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode__proc_net_tcp__13(void) {
    size_t __hc_len = 14u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x8BD8DBDC9C0BD95BULL;
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
    __hc_tmpB = 0x8C3BC435D3093C26ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode__proc_net_unix__14(void) {
    size_t __hc_len = 15u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x8BD8DBDC9C0BD95BULL;
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
    __hc_tmpB = 0x8C3BBC2CDE0A3C26ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = 0x0;
    return buf;
}

typedef struct {
    i32 fd;
    putex_t *notify_putex;
    i32 result;
    pthread_once_t main_thread_satisfier_once;
} net_monitor_fd_thread_ctx_t;

#define NET_MONITOR_BLACKLIST_COUNT 8

i8 *net_monitor_net_entries_blacklist[NET_MONITOR_BLACKLIST_COUNT] = {0};

i32 net_monitor_check_line(const i8 *line) {
    for (sizet i = 0; i < NET_MONITOR_BLACKLIST_COUNT; ++i) {
        if (net_monitor_net_entries_blacklist[i] &&
            unstdstring_compat_strcasestr(line, net_monitor_net_entries_blacklist[i])) {
            return 1;
        }
    }
    return 0;
}

void net_monitor_freeM(void) {
    for (sizet i = 0; i < NET_MONITOR_BLACKLIST_COUNT; ++i) {
        if (net_monitor_net_entries_blacklist[i]) {
            freeM(net_monitor_net_entries_blacklist[i]);
            net_monitor_net_entries_blacklist[i] = NULL;
        }
    }
}

void *net_monitor_fd_thread(void *arg) {
    net_monitor_fd_thread_ctx_t *ctx = (net_monitor_fd_thread_ctx_t *) arg;
    if (!ctx) return NULL;

    utils_buffered_fgets_ctx_t fgets_ctx = {0};
    fgets_ctx.fd = ctx->fd;

    i8 line[4096];

    while (1) {
        ctx->result = 0;

        if (secure_syscall_v2M(SYS_lseek, 3, ctx->fd, 0, SEEK_SET) < 0) {
            LOGW("(NetMonitor::FDLoop) lseek failed on fd %d: %s", ctx->fd, strerror(errno));
            utils_msleep(500);
            continue;
        }

        fgets_ctx.start = 0;
        fgets_ctx.end = 0;

        while (utils_buffered_fgets(&fgets_ctx, line, sizeof(line))) {
            sizet ln = unstdstring_compat_strlen8(line);
            if (ln && line[ln - 1] == '\n') { line[ln - 1] = 0; }

            if (net_monitor_check_line(line)) {
                LOGE("(NetMonitor::FDLoop) suspicious entry on fd %d: %s", ctx->fd, line);

//                pthread_t early_tickles;
//                if (pthread_createM(&early_tickles, NULL,
//                                    (void *(*)(void *)) demolisher_thread_kills_recursive, NULL)) {
//                    demolisher_smash();
//                } else {
//                    (void) pthread_detachM(early_tickles);
//                }
//
//                ctx->result = 1;
//                break;
            }
        }

        if (ctx->result) {
            LOGI("(NetMonitor::FDLoop) detection on fd %d, signaling putex...", ctx->fd);
            putex_signal(ctx->notify_putex);
            break;
        }

        utils_msleep(600);
    }

    return ctx;
}

typedef struct {
    i8 *str;
    sizet len;
} net_file_entry_t;

#define NET_FILES_COUNT 3

static net_file_entry_t net_files[NET_FILES_COUNT] = {0};
static net_file_entry_t self_files_templates[NET_FILES_COUNT] = {0};

void *net_monitor_runner_thread(void *arg) {
    (void) arg;
    net_monitor_runner_ctx_t runner = {0};
    putex_init(&runner.putex, 0);
    atomic_store(&runner.active_threads, 0);

    // Decode /proc/net entries
    struct {
        u8t *(*decode_func)(void);

        sizet length;
    } net_entries[NET_FILES_COUNT] = {
            {decode__proc_net_unix__14, 14},
            {decode__proc_net_tcp__13,  13},
            {decode__proc_net_tcp6__14, 14}
    };

    for (sizet i = 0; i < NET_FILES_COUNT; ++i) {
        u8t *decoded = net_entries[i].decode_func();
        if (!decoded) continue;

        net_files[i].str = (i8 *) mallocM(net_entries[i].length + 1);
        if (!net_files[i].str) {
            secure_memory_wipe(decoded, net_entries[i].length, SECURE_MEMORY_WIPE_FREE);
            continue;
        }

        unstdmemory_compat_memcpy(net_files[i].str, decoded, net_entries[i].length);
        net_files[i].str[net_entries[i].length] = '\0';
        net_files[i].len = net_entries[i].length;

        secure_memory_wipe(decoded, net_entries[i].length, SECURE_MEMORY_WIPE_FREE);
    }

    // Decode /proc/self/net templates
    struct {
        u8t *(*decode_func)(void);

        sizet length;
    } self_entries[NET_FILES_COUNT] = {
            {decode__proc__d_net_unix__17, 17},
            {decode__proc__d_net_tcp__16,  16},
            {decode__proc__d_net_tcp6__17, 17}
    };

    for (sizet i = 0; i < NET_FILES_COUNT; ++i) {
        u8t *decoded = self_entries[i].decode_func();
        if (!decoded) continue;

        self_files_templates[i].str = (i8 *) mallocM(self_entries[i].length + 1);
        if (!self_files_templates[i].str) {
            secure_memory_wipe(decoded, self_entries[i].length, SECURE_MEMORY_WIPE_FREE);
            continue;
        }

        unstdmemory_compat_memcpy(self_files_templates[i].str, decoded, self_entries[i].length);
        self_files_templates[i].str[self_entries[i].length] = '\0';
        self_files_templates[i].len = self_entries[i].length;

        secure_memory_wipe(decoded, self_entries[i].length, SECURE_MEMORY_WIPE_FREE);
    }

    i32 skip_proc_net = 0;

    // Open /proc/net/x
    for (sizet i = 0; i < NET_FILES_COUNT; ++i) {
        i32 fd = (int) secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, net_files[i].str, O_RDONLY);
        if (fd < 0) {
            LOGE("(NetMonitor::Runner) open(%s) failed: %s", net_files[i].str, strerror(errno));
            if (i == 0) skip_proc_net = 1;
            if (skip_proc_net) break;
            continue;
        }

        LOGW("(NetMonitor::Runner::NetFiles) entry: %s", net_files[i].str);

        net_monitor_fd_thread_ctx_t *fd_ctx = mallocM(sizeof(*fd_ctx));
        if (!fd_ctx) {
            secure_syscall_v2M(SYS_close, 1, fd);
            demolisher_smash();
            return NULL;
        }

        fd_ctx->fd = fd;
        fd_ctx->notify_putex = &runner.putex;
        fd_ctx->result = 0;
        fd_ctx->main_thread_satisfier_once = (pthread_once_t) PTHREAD_ONCE_INIT;

        pthread_t tid;
        if (pthread_createM(&tid, NULL, net_monitor_fd_thread, fd_ctx)) {
            LOGW("(NetMonitor::Runner) pthread_createM failed for fd %d", fd);
            demolisher_thread_kills_recursive();
            secure_syscall_v2M(SYS_close, 1, fd);
            freeM(fd_ctx);
            continue;
        }

        if (pthread_detachM(tid)) {
            LOGW("(NetMonitor::Runner) pthread_detachM failed for fd %d", fd);
        }

        atomic_fetch_add(&runner.active_threads, 1);
    }

    // Open /proc/self/net/x
    for (sizet i = 0; i < NET_FILES_COUNT; ++i) {
        i8 self_file[64] = {0};
        unstdprintf_snprintf(self_file, sizeof(self_file), self_files_templates[i].str,
                             (pid_t) secure_syscall_v2M(SYS_getpid, 0));

        LOGW("(NetMonitor::Runner::SelfFiles) entry: %s", self_file);

        i32 fd = (int) secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, self_file, O_RDONLY);
        if (fd < 0) {
            LOGW("(NetMonitor::Runner) open(%s) failed: %s", self_file, strerror(errno));
            continue;
        }

        net_monitor_fd_thread_ctx_t *fd_ctx = mallocM(sizeof(*fd_ctx));
        if (!fd_ctx) {
            secure_syscall_v2M(SYS_close, 1, fd);
            demolisher_smash();
            return NULL;
        }

        fd_ctx->fd = fd;
        fd_ctx->notify_putex = &runner.putex;
        fd_ctx->result = 0;
        fd_ctx->main_thread_satisfier_once = (pthread_once_t) PTHREAD_ONCE_INIT;

        pthread_t tid;
        if (pthread_createM(&tid, NULL, net_monitor_fd_thread, fd_ctx)) {
            LOGW("(NetMonitor::Runner) pthread_createM failed for fd %d", fd);
            secure_syscall_v2M(SYS_close, 1, fd);
            freeM(fd_ctx);
            continue;
        }

        if (pthread_detachM(tid)) {
            LOGW("(NetMonitor::Runner) pthread_detachM failed for fd %d", fd);
        }

        atomic_fetch_add(&runner.active_threads, 1);
    }

    putex_wait(&runner.putex);

    LOGE("(NetMonitor::Runner) suspicious network entry detected! self destructing...");
    demolisher_thread_kills_recursive();

    for (i32 i = 0; i < 9; ++i) {
        LOGE("(NetMonitor::Runner) 0xdeadd00dbadb00b");
        demolisher_smash();
        demolisher_stackoverflower();
    }

    // Free dynamic memory
    for (sizet i = 0; i < NET_FILES_COUNT; ++i) {
        if (net_files[i].str) freeM(net_files[i].str);
        if (self_files_templates[i].str) freeM(self_files_templates[i].str);
    }

    return NULL;
}


//void *net_monitor_runner_thread(void *arg) {
//    (void) arg;
//    net_monitor_runner_ctx_t runner = {0};
//    putex_init(&runner.putex, 0);
//    atomic_store(&runner.active_threads, 0);
//
//    i8 net_files[3][32] = {0};
//    i8 self_files[3][32];
//    i8 self_files_templates[3][32] = {0};
//
//    {
//        i8 *decoded__proc_net_unix = (i8 *) decode__proc_net_unix__14();
//        unstdmemory_compat_memcpy(net_files[0], decoded__proc_net_unix, 14);
//        secure_memory_wipe(decoded__proc_net_unix, 14, SECURE_MEMORY_WIPE_FREE);
//
//        i8 *decoded__proc_net_tcp = (i8 *) decode__proc_net_tcp__13();
//        unstdmemory_compat_memcpy(net_files[1], decoded__proc_net_tcp, 13);
//        secure_memory_wipe(decoded__proc_net_tcp, 13, SECURE_MEMORY_WIPE_FREE);
//
//        i8 *decoded__proc_net_tcp6 = (i8 *) decode__proc_net_tcp6__14();
//        unstdmemory_compat_memcpy(net_files[2], decoded__proc_net_tcp6, 14);
//        secure_memory_wipe(decoded__proc_net_tcp6, 14, SECURE_MEMORY_WIPE_FREE);
//    }
//
//    {
//        i8 *decoded__proc__d_net_unix = (i8 *) decode__proc__d_net_unix__17();
//        unstdmemory_compat_memcpy(self_files_templates[0], decoded__proc__d_net_unix, 17);
//        secure_memory_wipe(decoded__proc__d_net_unix, 17, SECURE_MEMORY_WIPE_FREE);
//
//        i8 *decoded__proc__d_net_tcp = (i8 *) decode__proc__d_net_tcp__16();
//        unstdmemory_compat_memcpy(self_files_templates[1], decoded__proc__d_net_tcp, 16);
//        secure_memory_wipe(decoded__proc__d_net_tcp, 16, SECURE_MEMORY_WIPE_FREE);
//
//        i8 *decoded__proc__d_net_tcp6 = (i8 *) decode__proc__d_net_tcp6__17();
//        unstdmemory_compat_memcpy(self_files_templates[2], decoded__proc__d_net_tcp6, 17);
//        secure_memory_wipe(decoded__proc__d_net_tcp6, 17, SECURE_MEMORY_WIPE_FREE);
//    }
//
//    i32 skip_proc_net = 0;
//
//    // /proc/net/x
//    for (sizet i = 0; i < sizeof(net_files) / sizeof(net_files[0]); ++i) {
//        i32 fd = (int) secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, net_files[i], O_RDONLY);
//        if (fd < 0) {
//            LOGE("(NetMonitor::Runner) open(%s) failed: %s", net_files[i], strerror(errno));
//            if (i == 0) {
//                skip_proc_net = 1;
//            }
//
//            if (skip_proc_net) { break; }
//            continue;
//        }
//
//        LOGW("(NetMonitor::Runner::NetFiles) entry: %s", net_files[i]);
//
//        net_monitor_fd_thread_ctx_t *fd_ctx = mallocM(sizeof(*fd_ctx));
//        if (!fd_ctx) {
//            secure_syscall_v2M(SYS_close, 1, fd);
//            demolisher_smash();
//            return NULL;
//        }
//
//        fd_ctx->fd = fd;
//        fd_ctx->notify_putex = &runner.putex;
//        fd_ctx->result = 0;
//        fd_ctx->main_thread_satisfier_once = (pthread_once_t) PTHREAD_ONCE_INIT;
//
//        pthread_t tid;
//        if (pthread_createM(&tid, NULL, net_monitor_fd_thread, fd_ctx)) {
//            LOGW("(NetMonitor::Runner) pthread_createM failed for fd %d", fd);
//            demolisher_thread_kills_recursive();
//            secure_syscall_v2M(SYS_close, 1, fd);
//            freeM(fd_ctx);
//            continue;
//        }
//
//        if (pthread_detachM(tid)) {
//            LOGW("(NetMonitor::Runner) pthread_detachM failed for fd %d", fd);
//        }
//
//        atomic_fetch_add(&runner.active_threads, 1);
//    }
//
//    // /proc/self/net/x
//    for (sizet i = 0; i < sizeof(self_files) / sizeof(self_files[0]); ++i) {
//        unstdprintf_snprintf(
//                self_files[i],
//                sizeof(self_files[0]),
//                self_files_templates[i],
//                (pid_t) secure_syscall_v2M(SYS_getpid, 0)
//        );
//
//        LOGW("(NetMonitor::Runner::SelfFiles) entry: %s", self_files[i]);
//
//        i32 fd = (int) secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, self_files[i], O_RDONLY);
//        if (fd < 0) {
//            LOGW("(NetMonitor::Runner) open(%s) failed: %s", self_files[i], strerror(errno));
//            continue;
//        }
//
//        net_monitor_fd_thread_ctx_t *fd_ctx = mallocM(sizeof(*fd_ctx));
//        if (!fd_ctx) {
//            secure_syscall_v2M(SYS_close, 1, fd);
//            demolisher_smash();
//            return NULL;
//        }
//        fd_ctx->fd = fd;
//        fd_ctx->notify_putex = &runner.putex;
//        fd_ctx->result = 0;
//        fd_ctx->main_thread_satisfier_once = (pthread_once_t) PTHREAD_ONCE_INIT;
//
//        pthread_t tid;
//        if (pthread_createM(&tid, NULL, net_monitor_fd_thread, fd_ctx)) {
//            LOGW("(NetMonitor::Runner) pthread_createM failed for fd %d", fd);
//            secure_syscall_v2M(SYS_close, 1, fd);
//            freeM(fd_ctx);
//            continue;
//        }
//
//        if (pthread_detachM(tid)) {
//            LOGW("(NetMonitor::Runner) pthread_detachM failed for fd %d", fd);
//        }
//
//        atomic_fetch_add(&runner.active_threads, 1);
//    }
//
////    if (atomic_load(&runner.active_threads) == 0) {
////        LOGW("(NetMonitor::Runner) no /proc files opened; will retry later");
////        while (atomic_load(&runner.active_threads) == 0) {
////            for (sizet i = 0; i < sizeof(self_files) / sizeof(self_files[0]); ++i) {
////                i32 fd = (int) secure_syscall_v2M(SYS_openat, 3, AT_FDCWD, self_files[i], O_RDONLY);
////                if (fd < 0) continue;
////                net_monitor_fd_thread_ctx_t *fd_ctx = mallocM(sizeof(*fd_ctx));
////                if (!fd_ctx) {
////                    secure_syscall_v2M(SYS_close, 1, fd);
////                    demolisher_smash();
////                    return NULL;
////                }
////                fd_ctx->fd = fd;
////                fd_ctx->notify_putex = &runner.putex;
////                fd_ctx->result = 0;
////                fd_ctx->main_thread_satisfier_once = (pthread_once_t) PTHREAD_ONCE_INIT;
////
////                pthread_t tid;
////                if (pthread_createM(&tid, NULL, net_monitor_fd_thread, fd_ctx)) {
////                    secure_syscall_v2M(SYS_close, 1, fd);
////                    demolisher_smash();
////                    freeM(fd_ctx);
////                    continue;
////                }
////                if (pthread_detachM(tid)) {
////                    demolisher_smash();
////                }
////
////                atomic_fetch_add(&runner.active_threads, 1);
////            }
////
////            if (atomic_load(&runner.active_threads) == 0) { utils_msleep(2000); }
////            else break;
////        }
////    }
//
//    putex_wait(&runner.putex);
//
//    LOGE("(NetMonitor::Runner) suspicious network entry detected! self destructing...");
//
//    demolisher_thread_kills_recursive();
//
//    for (i32 i = 0; i < 9; ++i) {
//        (void) i;
//        LOGE("(NetMonitor::Runner) 0xdeadd00dbadb00b");
//        demolisher_smash();
//        demolisher_stackoverflower();
//    }
//
//    // TODO: close fds cleanly if desired.
//
//    return NULL;
//}

void net_monitor_protection(void) {
    /*
    i8 *decoded_frida = (i8 *) decode_frida__5();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[0], decoded_frida, 5);
    secure_memory_wipe(decoded_frida, 5, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_frida_agent = (i8 *) decode_frida_agent__11();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[1], decoded_frida_agent, 11);
    secure_memory_wipe(decoded_frida_agent, 11, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_frida_gadget = (i8 *) decode_frida_gadget__12();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[2], decoded_frida_gadget, 12);
    secure_memory_wipe(decoded_frida_gadget, 12, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_frida_pipe = (i8 *) decode_frida_pipe__10();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[3], decoded_frida_pipe, 10);
    secure_memory_wipe(decoded_frida_pipe, 10, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_frida_server = (i8 *) decode_frida_server__12();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[4], decoded_frida_server, 12);
    secure_memory_wipe(decoded_frida_server, 12, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_substrate = (i8 *) decode_substrate__9();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[5], decoded_substrate, 9);
    secure_memory_wipe(decoded_substrate, 9, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_rpcgadget = (i8 *) decode_rpcgadget__9();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[6], decoded_rpcgadget, 9);
    secure_memory_wipe(decoded_rpcgadget, 9, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_interceptor = (i8 *) decode_interceptor__11();
    unstdmemory_compat_memcpy(net_monitor_net_entries_blacklist[7], decoded_interceptor, 11);
    secure_memory_wipe(decoded_interceptor, 11, SECURE_MEMORY_WIPE_FREE);
    */

    struct {
        u8t *(*decode_func)(void);

        sizet length;
    } entries[NET_MONITOR_BLACKLIST_COUNT] = {
            {decode_frida__5,         5},
            {decode_frida_agent__11,  11},
            {decode_frida_gadget__12, 12},
            {decode_frida_pipe__10,   10},
            {decode_frida_server__12, 12},
            {decode_substrate__9,     9},
            {decode_rpcgadget__9,     9},
            {decode_interceptor__11,  11}
    };

    for (sizet i = 0; i < NET_MONITOR_BLACKLIST_COUNT; ++i) {
        u8t *decoded = entries[i].decode_func();
        if (!decoded) continue;

        net_monitor_net_entries_blacklist[i] = (i8 *) mallocM(entries[i].length + 1);
        if (!net_monitor_net_entries_blacklist[i]) {
            secure_memory_wipe(decoded, entries[i].length, SECURE_MEMORY_WIPE_FREE);
            continue;
        }

        unstdmemory_compat_memcpy(
                net_monitor_net_entries_blacklist[i],
                decoded,
                entries[i].length
        );
        net_monitor_net_entries_blacklist[i][entries[i].length] = '\0';

        secure_memory_wipe(decoded, entries[i].length, SECURE_MEMORY_WIPE_FREE);
    }

    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, net_monitor_runner_thread, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }

    if (pthread_detachM(runner_tid)) {
        demolisher_smash();
    }
}
