//
// Created by johan on 06.10.25.
//
#include "code_integrity_gadget_royale_trusted.h"
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

static inline __attribute__((always_inline)) unsigned char *decode_libroyale_so__12(void) {
    size_t __hc_len = 13u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x5E5BDC989A5B1B18ULL;
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
    __hc_tmpB = 0x8C3BC445DF043D15ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = 0x0;
    return buf;
}

shemaphore_t code_integrity_gadget_royale_ensure_segment_readable_onetime_init_sem;
segments_t code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs;

static void code_integrity_gadget_royale_ensure_segment_readable_onetime_init(void) {
    if (!code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs.base ||
        code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs.end_of_exec <=
        code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) failed to locate libroyale.so segments");
        demolisher_smash();
        return;
    }

    u8t *start = code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs.base;
    u8t *end = code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs.end_of_exec;

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) trying to protect segment %p..%p",
         start, end);

    if (mprotectM(start, end - start, PROT_READ | PROT_EXEC) != 0) {
        LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) PROT_READ|EXEC failed, trying PROT_READ only");
        if (mprotectM(start, end - start, PROT_READ) != 0) {
            LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) PROT_READ also failed, segment may be partially unreadable!");
        } else {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) PROT_READ succeeded, segment is readable");
        }
    } else {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::EnsureSegmentReadableOnce) PROT_READ|EXEC succeeded");
    }

    shemaphore_signal(&code_integrity_gadget_royale_ensure_segment_readable_onetime_init_sem);
}


_Noreturn void *code_integrity_gadget_royale_hashchecker_blake3_loop(void *ctx_arg) {
    if (!ctx_arg) {
        demolisher_stackoverflower();
    }

    code_integrity_gadget_royale_trusted_ctx_t *ctx = (code_integrity_gadget_royale_trusted_ctx_t *) ctx_arg;

    if (!ctx
        || !ctx->libroyale_segments.base
        || ctx->libroyale_segments.end_of_exec <= ctx->libroyale_segments.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::BLAKE3Hashchecker::Loop) invalid context received.");
        demolisher_smash();
    }

    u8t current_hash[BLAKE3_OUT_LEN];
    sizet len = ctx->libroyale_segments.end_of_exec - ctx->libroyale_segments.base;
    blake3_hasher hasher;

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::BLAKE3Hashchecker::Loop) saved BLAKE3 hash for 0x%zx bytes (libroyale.so):",
         len);
    hexdump(ctx->trusted_hashset.blake3, BLAKE3_OUT_LEN);

    while (1) {
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, ctx->libroyale_segments.base, len);
        blake3_hasher_finalize(&hasher, current_hash, BLAKE3_OUT_LEN);

        if (unstdmemory_compat_memcmp(current_hash, ctx->trusted_hashset.blake3, BLAKE3_OUT_LEN) !=
            0) {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::BLAKE3Hashchecker::Loop) BLAKE3 mismatch detected!");
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::BLAKE3Hashchecker::Loop) current hash:");
            hexdump(current_hash, BLAKE3_OUT_LEN);
            demolisher_smash();
        }

        utils_msleep(985);
    }
}

void *code_integrity_gadget_royale_resolver_loop(void *ctx_arg) {
    if (!ctx_arg) {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Loop) ctx_arg missing");
        return NULL;
    }

    code_integrity_gadget_royale_trusted_ctx_t *ctx = (code_integrity_gadget_royale_trusted_ctx_t *) ctx_arg;

    i8 *decoded_libroyale_so = (i8 *) decode_libroyale_so__12();

    while (1) {
        segments_t segs = get_segments_ext(decoded_libroyale_so, SEG_SEARCH_PHDR, 1);

        if (segs.base) {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Loop) found libroyale.so: base=%p end_of_exec=%p size=%ld",
                 (void *) segs.base,
                 (void *) segs.end_of_exec,
                 segs.end_of_exec - segs.base);

            ctx->libroyale_segments.base = segs.base;
            ctx->libroyale_segments.end_of_exec = segs.end_of_exec;

            shemaphore_signal(&ctx->hashchecker_sem);
            break;
        } else {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Loop) NOT FOUND: base=%p | eox=%p",
                 (void *) segs.base,
                 (void *) (segs.end_of_exec));
        }

        utils_msleep(380);
    }

    secure_memory_wipe(decoded_libroyale_so, 11, SECURE_MEMORY_WIPE_FREE);

    return NULL;
}

void *code_integrity_gadget_royale_resolver_runner(void *arg) {
    (void) arg;

    code_integrity_gadget_royale_trusted_ctx_t *ctx = type_callocM(
            code_integrity_gadget_royale_trusted_ctx_t);

    shemaphore_init(&ctx->hashchecker_sem, 0);

    unstdmemory_compat_memcpy(
            &ctx->trusted_hashset,
            &TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0],
            sizeof(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0])
    );

    pthread_t loop_thread;
    if (pthread_createM(&loop_thread, NULL, code_integrity_gadget_royale_resolver_loop, ctx)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (pthread_detachM(loop_thread)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (shemaphore_wait_timeout(&ctx->hashchecker_sem, 16, 0) != SHEMAPHORE_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Runner) semaphore timeout — likely resolver failure");
        demolisher_smash();
        return NULL;
    }

    if (!ctx->libroyale_segments.base || !ctx->libroyale_segments.end_of_exec) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Runner) semaphore signaled but invalid segment range");
        return NULL;
    }

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Runner) libroyale.so discovered: base=%p end_of_exec=%p",
         (void *) ctx->libroyale_segments.base,
         (void *) ctx->libroyale_segments.end_of_exec);

    shemaphore_init(&code_integrity_gadget_royale_ensure_segment_readable_onetime_init_sem, 0);

    unstdmemory_compat_memcpy(
            &code_integrity_gadget_royale_ensure_segment_readable_onetime_init_segs,
            &ctx->libroyale_segments,
            sizeof(ctx->libroyale_segments)
    );

    pthread_onceM(&ctx->ensure_segment_readable_onetime_init_once,
                  code_integrity_gadget_royale_ensure_segment_readable_onetime_init);

    if (shemaphore_wait_timeout(
            &code_integrity_gadget_royale_ensure_segment_readable_onetime_init_sem, 3, 0) !=
        SHEMAPHORE_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Runner) segments ready semaphore timeout or failed");
        demolisher_smash();
        return NULL;
    }

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Royale::Trusted::Resolver::Runner) segments ready semaphore signal received.");

    pthread_t hashchecker_thread;
    if (pthread_createM(&hashchecker_thread,
                        NULL,
                        code_integrity_gadget_royale_hashchecker_blake3_loop,
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

void code_integrity_gadget_royale_trusted(void) {
    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, code_integrity_gadget_royale_resolver_runner, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }
}
