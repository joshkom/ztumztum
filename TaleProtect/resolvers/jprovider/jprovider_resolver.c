//
// Created by johan on 12.10.25.
//

#include "jprovider_resolver.h"
#include "TaleProtect/metas/jprovider.h"
#include "global_common/common.h"
#include "TaleProtect/resolvers/compat/metadata_resolver.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

static inline __attribute__((always_inline)) unsigned char *decode_libb_so__7(void) {
    size_t __hc_len = 8u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDCCB98989A5B001BULL;
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

void jprovider_resolver(void) {
    i8 *decoded_libb_so = (i8 *) decode_libb_so__7();
    LOGI("resolving jprovider %s...", decoded_libb_so);
    meta_jprovider = (JProvider_t *) find_metadata_by_magic_from_phdr(
            decoded_libb_so,
            meta_jprovider->magic,
            sizeof(JProvider_t)
    );

    secure_memory_wipe(decoded_libb_so, 7, SECURE_MEMORY_WIPE_FREE);

    LOGI("(JProvider) java vm -> 0x%lx", (uptrt) meta_jprovider->vm);
}
