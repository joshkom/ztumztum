//
// Created by johan on 05.10.25.
//
#include "code_integrity_gadget_tale_bootstrap.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/demolisher/stackoverflower.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/tools/putex/putex.h"
#include <TaleProtect/tools/segment_resolver/segment_resolver.h>
#include "TaleProtect/gcontext.h"

#include <openssl/digest.h>
#include <blake3.h>

putex_t code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init_putex;

static void code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init(void) {
    segments_t segs = get_segments_ext("libtale.so", SEG_SEARCH_PHDR, 1);
    if (!segs.base || segs.end_of_exec <= segs.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) failed to locate libtale.so segments");
        demolisher_smash();
        return;
    }

    u8t *start = segs.base;
    u8t *end = segs.end_of_exec;

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) trying to protect segment %p..%p",
         start, end);

    if (mprotectM(start, end - start, PROT_READ | PROT_EXEC) != 0) {
        LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) PROT_READ|EXEC failed, trying PROT_READ only");
        if (mprotectM(start, end - start, PROT_READ) != 0) {
            LOGW("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) PROT_READ also failed, segment may be partially unreadable!");
        } else {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) PROT_READ succeeded, segment is readable");
        }
    } else {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::EnsureSegmentReadableOnce) PROT_READ|EXEC succeeded");
    }

    putex_signal(&code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init_putex);
}

//_Noreturn void *code_integrity_gadget_tale_hashchecker_md5_loop(void *ctx_arg) {
//    if (!ctx_arg) {
//        demolisher_stackoverflower();
//    }
//
//    code_integrity_gadget_tale_bootstrap_ctx_t *ctx = (code_integrity_gadget_tale_bootstrap_ctx_t *) ctx_arg;
//
//    if (!ctx
//        || !ctx->libtale_segments.base
//        || ctx->libtale_segments.base
//           + TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.end_of_exec
//           <= ctx->libtale_segments.base) {
//        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::MD5Hashchecker::Loop) invalid context received.");
//        demolisher_smash();
//    }
//
//    u8t precomputed_md5[16];
//    u8t current_md5[16];
//
//    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
//    const EVP_MD *md = EVP_md5();
//
//    sizet len = TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.end_of_exec;
//
//    EVP_DigestInit_ex(mdctx, md, NULL);
//    EVP_DigestUpdate(mdctx, ctx->libtale_segments.base, len);
//    EVP_DigestFinal_ex(mdctx, precomputed_md5, NULL);
//
//    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::MD5Hashchecker::Loop) precomputed MD5 for 0x%zx bytes (libtale.so):",
//         len);
//
//    hexdump(precomputed_md5, 16);
//
//    while (1) {
//        EVP_DigestInit_ex(mdctx, md, NULL);
//        EVP_DigestUpdate(mdctx, ctx->libtale_segments.base, len);
//        EVP_DigestFinal_ex(mdctx, current_md5, NULL);
//
//        if (unstdmemory_compat_memcmp(current_md5, precomputed_md5, 16) != 0) {
//            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::MD5Hashchecker::Loop) MD5 mismatch detected!");
//            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::MD5Hashchecker::Loop) current MD5:");
//            hexdump(current_md5, 16);
//            demolisher_smash();
//        }
//
//        utils_msleep(5085);
//    }
//
//    EVP_MD_CTX_free(mdctx);
//}

_Noreturn void *code_integrity_gadget_tale_bootstrap_hashchecker_blake3_loop(void *ctx_arg) {
    if (!ctx_arg) {
        demolisher_stackoverflower();
    }

    code_integrity_gadget_tale_bootstrap_ctx_t *ctx = (code_integrity_gadget_tale_bootstrap_ctx_t *) ctx_arg;

    if (!ctx
        || !ctx->libtale_segments.base
        || ctx->libtale_segments.end_of_exec <= ctx->libtale_segments.base) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::BLAKE3Hashchecker::Loop) invalid context received.");
        demolisher_smash();
    }

    u8t precomputed_hash[BLAKE3_OUT_LEN];
    u8t current_hash[BLAKE3_OUT_LEN];

    sizet len = ctx->libtale_segments.end_of_exec - ctx->libtale_segments.base;

    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, ctx->libtale_segments.base, len);
    blake3_hasher_finalize(&hasher, precomputed_hash, BLAKE3_OUT_LEN);

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::BLAKE3Hashchecker::Loop) precomputed BLAKE3 hash for 0x%zx bytes (libtale.so):",
         len);
    hexdump(precomputed_hash, BLAKE3_OUT_LEN);

    while (1) {
        blake3_hasher_init(&hasher);
        blake3_hasher_update(&hasher, ctx->libtale_segments.base, len);
        blake3_hasher_finalize(&hasher, current_hash, BLAKE3_OUT_LEN);

        if (unstdmemory_compat_memcmp(current_hash, precomputed_hash, BLAKE3_OUT_LEN) != 0) {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::BLAKE3Hashchecker::Loop) BLAKE3 mismatch detected!");
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::BLAKE3Hashchecker::Loop) current hash:");
            hexdump(current_hash, BLAKE3_OUT_LEN);
            demolisher_smash();
        }

        utils_msleep(885);
//        utils_msleep(485);
    }
}

void *code_integrity_gadget_tale_bootstrap_resolver_loop(void *ctx_arg) {
    if (!ctx_arg) {
        LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Loop) ctx_arg missing");
        return NULL;
    }

    code_integrity_gadget_tale_bootstrap_ctx_t *ctx = (code_integrity_gadget_tale_bootstrap_ctx_t *) ctx_arg;

    while (1) {
        segments_t segs = get_segments_ext("libtale.so", SEG_SEARCH_PHDR, 1);

        if (segs.base) {
            LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Loop) found libtale.so: base=%p end_of_exec=%p size=%zx",
                 (void *) segs.base,
                 (void *) (segs.end_of_exec),
                 ctx->libtale_segments.end_of_exec - ctx->libtale_segments.base);

            ctx->libtale_segments.base = segs.base;
            ctx->libtale_segments.end_of_exec = segs.end_of_exec;

            putex_signal(&ctx->hashchecker_invoker);
            break;
        } else {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Loop) NOT FOUND: base=%p | eox=%p",
                 (void *) segs.base,
                 (void *) (segs.end_of_exec));
        }

        utils_msleep(280);
    }

    return NULL;
}

void *code_integrity_gadget_tale_bootstrap_resolver_runner(void *arg) {
    (void) arg;

    code_integrity_gadget_tale_bootstrap_ctx_t *ctx = type_callocM(code_integrity_gadget_tale_bootstrap_ctx_t);
    putex_init(&ctx->hashchecker_invoker, 0);

    pthread_t loop_thread;
    if (pthread_createM(&loop_thread, NULL, code_integrity_gadget_tale_bootstrap_resolver_loop, ctx)) {
        demolisher_stackoverflower();
        return NULL;
    }

    if (pthread_detachM(loop_thread)) {
        demolisher_stackoverflower();
        return NULL;
    }

    putex_result_t ptx_rv__hashchecker_invoker = putex_wait_timeout(
            &ctx->hashchecker_invoker,
            12,
            0
    );

    if (ptx_rv__hashchecker_invoker != PUTEX_ACQUIRED) {
        if (ptx_rv__hashchecker_invoker == PUTEX_TIMEOUT) {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) putex timeout — likely resolver failure");
        } else {
            LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) unexpected putex result");
        }
        demolisher_smash();
        return NULL;
    }

    if (!ctx->libtale_segments.base || !ctx->libtale_segments.end_of_exec) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) putex signaled but invalid segment range");
        return NULL;
    }

    LOGI("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) libtale.so discovered: base=%p end_of_exec=%p",
         (void *) ctx->libtale_segments.base,
         (void *) ctx->libtale_segments.end_of_exec);

    putex_init(&code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init_putex, 0);

    pthread_onceM(
            &ctx->ensure_segment_readable_onetime_init_once,
            code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init
    );

    i32 ptx_rv__ensure_segment_readable_onetime_init_once = putex_wait_timeout(
            &code_integrity_gadget_tale_bootstrap_ensure_segment_readable_onetime_init_putex,
            3,
            0
    );

    if (ptx_rv__ensure_segment_readable_onetime_init_once != PUTEX_ACQUIRED) {
        LOGE("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) segments ready putex timeout or failed via putex_result_t(%d)",
             ptx_rv__ensure_segment_readable_onetime_init_once);
        demolisher_smash();
        return NULL;
    }

    LOGD("(IntegrityMonitor::CodeIntegrity::External::Gadgets::Tale::Bootstrap::Resolver::Runner) segments ready putex signal received.");

    pthread_t hashchecker_thread;
    if (pthread_createM(&hashchecker_thread,
                        NULL,
//                        code_integrity_gadget_tale_hashchecker_md5_loop,
                        code_integrity_gadget_tale_bootstrap_hashchecker_blake3_loop,
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


void code_integrity_gadget_tale_bootstrap(void) {
    pthread_t runner_tid;
    if (pthread_createM(&runner_tid, NULL, code_integrity_gadget_tale_bootstrap_resolver_runner, NULL)) {
        demolisher_smash();
        demolisher_stackoverflower();
    }

    if (pthread_detachM(runner_tid)) {
        demolisher_smash();
    }
}
