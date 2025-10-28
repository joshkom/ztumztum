//
// Created by johan on 06.10.25.
//
#include "code_integrity_gadget_tale_trusted.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/demolisher/stackoverflower.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/tools/shemaphore/shemaphore.h"
#include <TaleProtect/tools/segment_resolver/segment_resolver.h>
#include "TaleProtect/gcontext.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"

#include <openssl/digest.h>
#include <blake3.h>

shemaphore_t code_integrity_gadget_tale_ensure_segment_readable_onetime_init_sem;

static inline __attribute__((always_inline)) unsigned char *decode_libtale_so__10(void) {
    size_t __hc_len = 11u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x5B185D189A5B0B99ULL;
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
    __hc_tmpB = 0x8C3BC4456FD4FC23ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = 0x0;
    return buf;
}

static void code_integrity_gadget_tale_ensure_segment_readable_onetime_init(void) {
    i8 *decoded_libtale_so = (i8 *) decode_libtale_so__10();
    segments_t segs = get_segments_ext(decoded_libtale_so, SEG_SEARCH_PHDR, 1);
    if (!segs.base || segs.end_of_exec <= segs.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) failed to locate libtale.so segments");
        secure_memory_wipe(decoded_libtale_so, 10, SECURE_MEMORY_WIPE_FREE);
        demolisher_smash();
        return;
    }

    secure_memory_wipe(decoded_libtale_so, 10, SECURE_MEMORY_WIPE_FREE);

    u8t *start = segs.base;
    u8t *end = segs.end_of_exec;

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) trying to protect segment %p..%p",
         start, end);

    if (mprotectM(start, end - start, PROT_READ | PROT_EXEC) != 0) {
        LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) PROT_READ|EXEC failed, trying PROT_READ only");
        if (mprotectM(start, end - start, PROT_READ) != 0) {
            LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) PROT_READ also failed, segment may be partially unreadable!");
        } else {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) PROT_READ succeeded, segment is readable");
        }
    } else {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::EnsureSegmentReadableOnce) PROT_READ|EXEC succeeded");
    }

    shemaphore_signal(&code_integrity_gadget_tale_ensure_segment_readable_onetime_init_sem);
}

_Noreturn void *code_integrity_gadget_tale_hashchecker_blake3_loop(void *ctx_arg) {
    if (!ctx_arg) {
        demolisher_stackoverflower();
    }

    code_integrity_gadget_tale_trusted_ctx_t *ctx = (code_integrity_gadget_tale_trusted_ctx_t *) ctx_arg;

    if (!ctx || !ctx->libtale_segments.base ||
        ctx->libtale_segments.end_of_exec <= ctx->libtale_segments.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::BLAKE3Hashchecker::Loop) invalid context received.");
        demolisher_smash();
    }

    u8t current_hash[BLAKE3_OUT_LEN];
    sizet len = ctx->libtale_segments.end_of_exec - ctx->libtale_segments.base;
    blake3_hasher hasher;

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::BLAKE3Hashchecker::Loop) saved BLAKE3 hash for 0x%zx bytes (libtale.so):",
         len);
    hexdump(ctx->trusted_hashset.blake3, BLAKE3_OUT_LEN);

    while (1) {
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, ctx->libtale_segments.base, len);
        blake3_hasher_finalize(&hasher, current_hash, BLAKE3_OUT_LEN);

        if (unstdmemory_compat_memcmp(current_hash, ctx->trusted_hashset.blake3, BLAKE3_OUT_LEN) !=
            0) {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::BLAKE3Hashchecker::Loop) BLAKE3 mismatch detected!");
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::BLAKE3Hashchecker::Loop) current hash:");
            hexdump(current_hash, BLAKE3_OUT_LEN);
            demolisher_smash();
        }

        utils_msleep(985);
    }
}

void *code_integrity_gadget_tale_resolver_loop(void *ctx_arg) {
    if (!ctx_arg) {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Loop) ctx_arg missing");
        return NULL;
    }

    code_integrity_gadget_tale_trusted_ctx_t *ctx = (code_integrity_gadget_tale_trusted_ctx_t *) ctx_arg;
    i8 *decoded_libtale_so = (i8 *) decode_libtale_so__10();

    while (1) {
        segments_t segs = get_segments_ext(decoded_libtale_so, SEG_SEARCH_PHDR, 1);

        if (segs.base) {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Loop) found libtale.so: base=%p end_of_exec=%p size=%zx",
                 (void *) segs.base,
                 (void *) segs.end_of_exec,
                 ctx->libtale_segments.end_of_exec - ctx->libtale_segments.base);

            ctx->libtale_segments.base = segs.base;
            ctx->libtale_segments.end_of_exec = segs.end_of_exec;

            shemaphore_signal(&ctx->hashchecker_sem);
            break;
        } else {
            LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Loop) NOT FOUND: base=%p | eox=%p",
                 (void *) segs.base,
                 (void *) segs.end_of_exec);
        }

        utils_msleep(380);
    }

    secure_memory_wipe(decoded_libtale_so, 10, SECURE_MEMORY_WIPE_FREE);

    return NULL;
}

void *code_integrity_gadget_tale_resolver_runner(void *arg) {
    (void) arg;

    code_integrity_gadget_tale_trusted_ctx_t *ctx = type_callocM(
            code_integrity_gadget_tale_trusted_ctx_t);

    shemaphore_init(&ctx->hashchecker_sem, 0);

    unstdmemory_compat_memcpy(
            &ctx->trusted_hashset,
            &TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0],
            sizeof(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0])
    );

    pthread_t loop_thread;
    if (pthread_createM(&loop_thread, NULL, code_integrity_gadget_tale_resolver_loop, ctx)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (pthread_detachM(loop_thread)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (shemaphore_wait_timeout(&ctx->hashchecker_sem, 23, 0) != SHEMAPHORE_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Runner) semaphore timeout — likely resolver failure");
        demolisher_smash();
        return NULL;
    }

    if (!ctx->libtale_segments.base || !ctx->libtale_segments.end_of_exec) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Runner) semaphore signaled but invalid segment range");
        return NULL;
    }

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Runner) libtale.so discovered: base=%p end_of_exec=%p",
         (void *) ctx->libtale_segments.base,
         (void *) ctx->libtale_segments.end_of_exec);

    shemaphore_init(&code_integrity_gadget_tale_ensure_segment_readable_onetime_init_sem, 0);

    pthread_onceM(&ctx->ensure_segment_readable_onetime_init_once,
                  code_integrity_gadget_tale_ensure_segment_readable_onetime_init);

    if (shemaphore_wait_timeout(
            &code_integrity_gadget_tale_ensure_segment_readable_onetime_init_sem, 5, 0) !=
        SHEMAPHORE_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Runner) segments ready semaphore timeout or failed");
        demolisher_smash();
        return NULL;
    }

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Trusted::Resolver::Runner) segments ready semaphore signal received.");

    pthread_t hashchecker_thread;
    if (pthread_createM(&hashchecker_thread,
                        NULL,
                        code_integrity_gadget_tale_hashchecker_blake3_loop,
                        ctx)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (pthread_detachM(hashchecker_thread)) {
        demolisher_stackoverflower();
        return NULL;
    }

    return NULL;
}

void code_integrity_gadget_tale_trusted(void) {
    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, code_integrity_gadget_tale_resolver_runner, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }
}
