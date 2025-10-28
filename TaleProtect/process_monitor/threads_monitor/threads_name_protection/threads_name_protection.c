#include "threads_name_protection.h"

#include <linux/fcntl.h>
#include <unstdstring_compat.h>
#include <unstdprintf_compat.h>

#include "TaleProtect/tools/utils.h"
#include "TaleProtect/demolisher/threads_kill_recursive.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/indirect_registration/indirect_registration.h"

//! /proc/self/task
static inline __attribute__((always_inline)) unsigned char *decode_proc_self_task__15(void) {
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

//! /proc/self/task/%s/comm
static inline __attribute__((always_inline)) unsigned char *decode_proc_self_task__comm__23(void) {
    size_t __hc_len = 24u;
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
    __hc_tmpB = 0xA350B724E3BFF51EULL;
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
    __hc_tmpB = 0xC9401B5B387A3E12ULL;
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
    buf[23] = 0x0;
    return buf;
}

//! frida
static inline __attribute__((always_inline)) u8t *decode_frida__5(void) {
    size_t __hc_len = 6u;
    u8t *buf = (u8t *) mallocM(__hc_len);
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
    buf[0] = (u8t) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (u8t) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (u8t) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (u8t) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (u8t) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = 0x0;
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

//! gmain
static inline __attribute__((always_inline)) unsigned char *decode_gmain__5(void) {
    size_t __hc_len = 6u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x001B9A585B59C000ULL;
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

//! gum-js
static inline __attribute__((always_inline)) unsigned char *decode_gum_js__6(void) {
    size_t __hc_len = 7u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x1CDA8B5B5D59C000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = 0x0;
    return buf;
}

//! dbus (maybe just check for gdbus for less false-positive rate)
static inline __attribute__((always_inline)) unsigned char *decode_dbus__4(void) {
    size_t __hc_len = 5u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x00001CDD58990000ULL;
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

//! gadget
static inline __attribute__((always_inline)) unsigned char *decode_gadget__6(void) {
    size_t __hc_len = 7u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x1D1959D91859C000ULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = 0x0;
    return buf;
}

//! dobby
static inline __attribute__((always_inline)) unsigned char *decode_dobby__5(void) {
    size_t __hc_len = 6u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x001E58989BD90000ULL;
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

//! stalker
static inline __attribute__((always_inline)) unsigned char *decode_stalker__7(void) {
    size_t __hc_len = 8u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x995ADB185D1CC01CULL;
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
    buf[7] = 0x0;
    return buf;
}

struct linux_dirent64 {
    uint64_t d_ino;    /* inode number */
    int64_t d_off;    /* offset to next linux_dirent64 */
    unsigned short d_reclen; /* length of this linux_dirent64 */
    u8t d_type;   /* file type */
    i8 d_name[]; /* filename (null-terminated) */
};

#define THREADS_NAME_BLACKLIST_COUNT 9

i8 *threads_name_protection_thread_names_blacklist[THREADS_NAME_BLACKLIST_COUNT] = {0};

i32 threads_name_protection_check_blacklist(const i8 *line) {
    for (sizet i = 0; i < THREADS_NAME_BLACKLIST_COUNT; ++i) {
        if (threads_name_protection_thread_names_blacklist[i] &&
            unstdstring_compat_strcasestr(
                    line,
                    threads_name_protection_thread_names_blacklist[i])) {
            return 1;
        }
    }
    return 0;
}

void threads_name_protection_free(void) {
    for (sizet i = 0; i < THREADS_NAME_BLACKLIST_COUNT; ++i) {
        if (threads_name_protection_thread_names_blacklist[i]) {
            freeM(threads_name_protection_thread_names_blacklist[i]);
            threads_name_protection_thread_names_blacklist[i] = NULL;
        }
    }
}

#define THREAD_NAMES_PROTECTION_BUF_SIZE 4096

//void *threads_name_protection_loop(void *arg) {
//    (void) arg;
//
//    i8 proc_self_task_holder[16] = {0};
//    i8 *decoded_proc_self_task = (i8 *) decode_proc_self_task__15();
//    unstdmemory_compat_memcpy(proc_self_task_holder, decoded_proc_self_task, 15);
//    LOGD("PATH>>>>>>>>> %s", proc_self_task_holder);
//
//    u8t buf[THREAD_NAMES_PROTECTION_BUF_SIZE];
//
//    while (1) {
//        i32 fd = (i32) secure_syscall_v2M(
//                SYS_openat, 4,
//                AT_FDCWD,
//                "/proc/self/task",
//                O_RDONLY | O_DIRECTORY,
//                0
//        );
//
//        if (fd < 0) {
//            LOGW("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) openat failed to open \"%s\": %d",
//                 proc_self_task_holder, fd);
//            utils_msleep(500);
//            continue;
//        }
//
//        ssizet nread;
//        while ((nread = secure_syscall_v2M(
//                SYS_getdents64, 3,
//                fd,
//                buf,
//                THREAD_NAMES_PROTECTION_BUF_SIZE)) > 0) {
//            sizet bpos = 0;
//            while (bpos < (sizet) nread) {
//                struct linux_dirent64 *d = (struct linux_dirent64 *) (buf + bpos);
//                bpos += d->d_reclen;
//                if (unstdstring_compat_strlen8(d->d_name) <= 2 &&
//                    d->d_name[0] == '.') {
//                    continue;
//                }  /* skip "." and ".." */
//
//
//                i8 comm_path[128];
//
//                unstdprintf_snprintf(
//                        comm_path,
//                        sizeof(comm_path),
//                        "/proc/self/task/%s/comm",
//                        d->d_name
//                );
//
////                LOGD("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection) entry: \"%s\"",
////                     comm_path);
//
//                i32 comm_fd = (i32) secure_syscall_v2M(
//                        SYS_openat,
//                        4,
//                        AT_FDCWD,
//                        comm_path,
//                        O_RDONLY,
//                        0
//                );
//
//                if (comm_fd < 0) { continue; }
//
//                i8 comm[64] = {0};
//                ssizet n = secure_syscall_v2M(SYS_read, 3, comm_fd, comm, sizeof(comm) - 1);
//                secure_syscall_v2M(SYS_close, 1, comm_fd);
//
//                if (n > 0) {
//                    if (comm[n - 1] == '\n') { comm[n - 1] = 0; }
//                    if (threads_name_protection_check_blacklist(comm)) {
//                        LOGE("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) suspicious thread detected: \"%s\" within \"%s\"",
//                             comm, comm_path);
//
////                        todo: uncomment these before release!
////                        pthread_t tid;
////                        if (pthread_create(&tid, NULL,
////                                           (void *(*)(void *)) demolisher_thread_kills_recursive,
////                                           NULL)) {
////                            demolisher_smash();
////                        } else {
////                            pthread_detach(tid);
////                        }
////
//////                        putex_signal(&putex);
////                        secure_syscall_v2M(SYS_close, 1, fd);
////                        return NULL;
//                    }
//                }
//            }
//        }
//
//        secure_memory_wipe(decoded_proc_self_task, 15, SECURE_MEMORY_WIPE_FREE);
//        secure_syscall_v2M(SYS_close, 1, fd);
//        utils_msleep(200);
//    }
//
//    return NULL;
//}

pthread_once_t indirect_registration_registration_thread_func_ptr_setter_invoker_once;

void *threads_name_protection_loop(void *arg) {
    (void) arg;

    i8 *decoded_proc_self_task = (i8 *) decode_proc_self_task__15();
    i8 *decoded_proc_self_task__comm = (i8 *) decode_proc_self_task__comm__23();

    u8t buf[THREAD_NAMES_PROTECTION_BUF_SIZE];

    while (1) {
        i32 fd = (i32) secure_syscall_v2M(
                SYS_openat, 4,
                AT_FDCWD,
                decoded_proc_self_task,
                O_RDONLY | O_DIRECTORY,
                0
        );

        if (fd < 0) {
            LOGW("(ProcessMonitor::ThreadsNameProtection) openat failed: %d", fd);
//            LOGW("(ProcessMonitor::ThreadsNameProtection) openat \"%s\" failed: %d",
//                 proc_self_task_holder, fd);
            utils_msleep(500);
            continue;
        }

        ssizet nread;
        while ((nread = secure_syscall_v2M(
                SYS_getdents64, 3,
                fd,
                buf,
                THREAD_NAMES_PROTECTION_BUF_SIZE)) > 0) {

            size_t bpos = 0;
            while (bpos + sizeof(struct linux_dirent64) <= (size_t) nread) {
                struct linux_dirent64 tmp_hdr;
                size_t header_sz = offsetof(struct linux_dirent64, d_name);
                if (bpos + header_sz > (size_t) nread) break;
                unstdmemory_compat_memcpy(&tmp_hdr, buf + bpos, header_sz);

                unsigned short reclen = tmp_hdr.d_reclen;
                if (reclen == 0) {
                    LOGW("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) getdents returned reclen==0, aborting parse");
                    break;
                }
                if (bpos + reclen > (size_t) nread) {
                    LOGW("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) reclen out of bounds (bpos=%zu reclen=%u nread=%zd)",
                         bpos, (unsigned) reclen, nread);
                    break;
                }

                const char *name_ptr = (const char *) (buf + bpos + header_sz);
                size_t name_max = reclen - header_sz;
                char dname[256];
                size_t copy_len = name_max < sizeof(dname) - 1 ? name_max : sizeof(dname) - 1;
                unstdmemory_compat_memcpy(dname, name_ptr, copy_len);
                dname[copy_len] = '\0';

                if (dname[0] == '.'
                    && (dname[1] == '\0'
                        || (dname[1] == '.'
                            && dname[2] == '\0'))) {
                    bpos += reclen;
                    continue;
                }

                char comm_path[128];
                int rc = unstdprintf_snprintf(
                        comm_path,
                        sizeof(comm_path),
                        decoded_proc_self_task__comm,
                        dname
                );

                if (rc < 0 || (size_t) rc >= sizeof(comm_path)) {
                    LOGW("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) comm_path truncated, skipping: %s",
                         dname);
                    bpos += reclen;
                    continue;
                }

                i32 comm_fd = (i32) secure_syscall_v2M(
                        SYS_openat,
                        4,
                        AT_FDCWD,
                        comm_path,
                        O_RDONLY,
                        0
                );

                if (comm_fd < 0) {
                    bpos += reclen;
                    continue;
                }

                char comm[64] = {0};
                ssizet n = secure_syscall_v2M(SYS_read, 3, comm_fd, comm, sizeof(comm) - 1);
                secure_syscall_v2M(SYS_close, 1, comm_fd);

                if (n > 0) {
                    if ((size_t) n >= sizeof(comm)) {
                        comm[sizeof(comm) - 1] = '\0';
                    } else if (comm[n - 1] == '\n') {
                        comm[n - 1] = '\0';
                    } else {
                        comm[n] = '\0';
                    }

                    if (threads_name_protection_check_blacklist(comm)) {
                        LOGE("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) suspicious thread \"%s\" in \"%s\"",
                             comm, comm_path);

//                        todo: uncomment these before release!
                        pthread_t tid;
                        if (pthread_createM(&tid, NULL,
                                            (void *(*)(void *)) demolisher_thread_kills_recursive,
                                            NULL)) {
                            demolisher_smash();
                        } else {
                            pthread_detachM(tid);
                        }

                        demolisher_thread_kills_recursive();
                        demolisher_smash();
//                        putex_signal(&putex);
                        secure_syscall_v2M(SYS_close, 1, fd);
                        return NULL;
                    }
                }

                bpos += reclen;
            } /* end iterate dir entries */

            pthread_onceM(
                    &indirect_registration_registration_thread_func_ptr_setter_invoker_once,
                    indirect_registration_registration_thread_func_ptr_setter_invoker
            );

        } /* end while getdents */

        if (nread < 0) {
            LOGW("(ProcessMonitor::ThreadsMonitor::ThreadsNameProtection::Loop) getdents failed: %s",
                 strerror(errno));
        }

        secure_syscall_v2M(SYS_close, 1, fd);
        utils_msleep(386);
    } /* end outer while */

    secure_memory_wipe(decoded_proc_self_task__comm, 23, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_proc_self_task, 15, SECURE_MEMORY_WIPE_FREE);

    return NULL;
}


void threads_name_protection(void) {
    /*
    i8 *decoded_frida = (i8 *) decode_frida__5();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[0], decoded_frida, 5);
    secure_memory_wipe(decoded_frida, 5, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_gmain = (i8 *) decode_gmain__5();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[1], decoded_gmain, 5);
    secure_memory_wipe(decoded_gmain, 5, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_gum_js = (i8 *) decode_gum_js__6();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[2], decoded_gum_js, 6);
    secure_memory_wipe(decoded_gum_js, 6, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_gadget = (i8 *) decode_gadget__6();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[3], decoded_gadget, 6);
    secure_memory_wipe(decoded_gadget, 6, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_stalker = (i8 *) decode_stalker__7();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[4], decoded_stalker,
                              7);
    secure_memory_wipe(decoded_stalker, 7, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_dbus = (i8 *) decode_dbus__4();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[5], decoded_dbus, 4);
    secure_memory_wipe(decoded_dbus, 4, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_dobby = (i8 *) decode_dobby__5();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[6], decoded_dobby, 5);
    secure_memory_wipe(decoded_dobby, 5, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_substrate = (i8 *) decode_substrate__9();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[7], decoded_substrate,
                              9);
    secure_memory_wipe(decoded_substrate, 9, SECURE_MEMORY_WIPE_FREE);

    i8 *decoded_interceptor = (i8 *) decode_interceptor__11();
    unstdmemory_compat_memcpy(threads_name_protection_thread_names_blacklist[8],
                              decoded_interceptor, 11);
    secure_memory_wipe(decoded_interceptor, 11, SECURE_MEMORY_WIPE_FREE);
    */

    struct {
        i8 *(*decode_func)(void);

        sizet length;
    } entries[THREADS_NAME_BLACKLIST_COUNT] = {
            {(i8 *(*)(void)) decode_frida__5,        5},
            {(i8 *(*)(void)) decode_gmain__5,        5},
            {(i8 *(*)(void)) decode_gum_js__6,       6},
            {(i8 *(*)(void)) decode_gadget__6,       6},
            {(i8 *(*)(void)) decode_stalker__7,      7},
            {(i8 *(*)(void)) decode_dbus__4,         4},
            {(i8 *(*)(void)) decode_dobby__5,        5},
            {(i8 *(*)(void)) decode_substrate__9,    9},
            {(i8 *(*)(void)) decode_interceptor__11, 11}
    };

    for (sizet i = 0; i < THREADS_NAME_BLACKLIST_COUNT; ++i) {
        i8 *decoded = entries[i].decode_func();
        if (!decoded) continue;

        threads_name_protection_thread_names_blacklist[i] = (i8 *) mallocM(entries[i].length + 1);
        if (!threads_name_protection_thread_names_blacklist[i]) {
            secure_memory_wipe(decoded, entries[i].length, SECURE_MEMORY_WIPE_FREE);
            continue;
        }

        unstdmemory_compat_memcpy(
                threads_name_protection_thread_names_blacklist[i],
                decoded,
                entries[i].length
        );
        threads_name_protection_thread_names_blacklist[i][entries[i].length] = '\0';

        secure_memory_wipe(decoded, entries[i].length, SECURE_MEMORY_WIPE_FREE);
    }

    pthread_t tid;
    if (pthread_createM(&tid, NULL, threads_name_protection_loop, NULL)) {
        demolisher_smash();
    }

    if (pthread_detachM(tid)) {
        demolisher_stackoverflower();
    }
}
