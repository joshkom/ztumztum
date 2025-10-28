#include "code_integrity_bootstrap_0.h"
#include "TaleProtect/demolisher/rainbow_terminator.h"
#include "TaleProtect/tools/putex/putex.h"
#include <openssl/digest.h>

//typedef struct {
//    struct {
//        u8t sha256[32];
//        u8t md5[16];
//    } bootstrap_hashes;
//} code_integrity_bootstrap_0_ctx_t;

pthread_once_t code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_once = PTHREAD_ONCE_INIT;
putex_t code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_putex;

//void code_integrity_bootstrap_0_ensure_segment_readable_onetime_init(void) {
//    segments_t segs = get_segments(
//            (void *) code_integrity_bootstrap_0_ensure_segment_readable_onetime_init
//    );
//
//    sizet page_size = (sizet) sysconfM(_SC_PAGESIZE);
//    if (page_size == (sizet) -1) page_size = 4096;
//
//    uptrt start = (uptrt) segs.exec.start;
//    uptrt end = start + segs.exec.size - 1;
//
//    void *mstart = (void *) (start & ~(page_size - 1));
//    sizet mlen = (sizet) ((end - (uptrt) mstart) + 1);
//
//    if (mlen % page_size) {
//        mlen = ((mlen / page_size) + 1) * page_size;
//    }
//
//    if (mprotect(mstart, mlen, PROT_READ | PROT_EXEC) != 0) {
//        LOGE("(CodeIntegrity::Bootstrap0) ensure_exec_segment_readable_once: mprotect failed");
//    } else {
//        LOGI("(CodeIntegrity::Bootstrap0) segment %p..%p is now READ|EXEC (one-time)",
//             mstart,
//             (void *) ((uptrt) mstart + mlen));
//    }
//
//    putex_signal(&code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_putex);
//}

void code_integrity_bootstrap_0_ensure_segment_readable_onetime_init(void) {
    segments_t segs = get_segments(
            (void *) code_integrity_bootstrap_0_ensure_segment_readable_onetime_init
    );

    sizet page_size = (sizet) sysconfM(_SC_PAGESIZE);
    if (page_size == (sizet) -1) page_size = 4096;

    u8t *start = segs.base;
    u8t *end = segs.end_of_exec - 1;

    void *mstart = (void *) ((uptrt) start & ~(page_size - 1));
    sizet mlen = (sizet) ((uptrt) end - (uptrt) mstart + 1);

    if (mlen % page_size) {
        mlen = ((mlen / page_size) + 1) * page_size;
    }

    if (mprotectM(mstart, mlen, PROT_READ | PROT_EXEC) != 0) {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::EnsureSegmentReadableOnce) ensure_segment_readable_once: mprotect failed");
    } else {
        LOGI("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::EnsureSegmentReadableOnce) segment %p..%p is now READ|EXEC (one-time)",
             mstart,
             (void *) ((uptrt) mstart + mlen));
    }

    putex_signal(&code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_putex);
}

_Noreturn void *code_integrity_bootstrap_0_md5_thread(void *arg) {
    u8t precomputed_md5[16];

    segments_t segs = get_segments((void *) code_integrity_bootstrap_0_md5_thread);
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();

    sizet len = (sizet) (segs.end_of_exec - segs.base);

    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, segs.base, len);
    EVP_DigestFinal_ex(ctx, precomputed_md5, NULL);

    LOGD("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::MD5Thread) pre-computed md5 for 0x%zx bytes:", len);
    hexdump(precomputed_md5, 16);

    u8t current_md5[16];
    while (1) {
        EVP_DigestInit_ex(ctx, md, NULL);
        EVP_DigestUpdate(ctx, segs.base, len);
        EVP_DigestFinal_ex(ctx, current_md5, NULL);

        if (unstdmemory_compat_memcmp(current_md5, precomputed_md5, 16) != 0) {
            LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::MD5Thread) MD5 hash mismatch detected!");
            LOGW("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::MD5Thread) current MD5:");
            hexdump(current_md5, 16);
        }

        utils_msleep(360);
    }

    EVP_MD_CTX_free(ctx);
}

_Noreturn void *code_integrity_bootstrap_0_sha256_thread(void *arg) {
    u8t precomputed_sha256[32];

    segments_t segs = get_segments((void *) code_integrity_bootstrap_0_sha256_thread);
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

    sizet len = (sizet) (segs.end_of_exec - segs.base);

    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, segs.base, len);
    EVP_DigestFinal_ex(ctx, precomputed_sha256, NULL);

    LOGD("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::SHA256Thread) pre-computed sha256 for 0x%zx bytes:", len);
    hexdump(precomputed_sha256, 32);

    u8t current_sha256[32];
    while (1) {
        EVP_DigestInit_ex(ctx, md, NULL);
        EVP_DigestUpdate(ctx, segs.base, len);
        EVP_DigestFinal_ex(ctx, current_sha256, NULL);

        if (unstdmemory_compat_memcmp(current_sha256, precomputed_sha256, 32) != 0) {
            LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::SHA256Thread) hash mismatch detected!");
            LOGW("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0::SHA256Thread) current SHA256:");
            hexdump(current_sha256, 32);
        }

        utils_msleep(820);
    }

    EVP_MD_CTX_free(ctx);
}


void code_integrity_bootstrap_0(void) {
    putex_init(&code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_putex, 0);

    pthread_onceM(
            &code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_once,
            code_integrity_bootstrap_0_ensure_segment_readable_onetime_init
    );

    i32 putex_rv = putex_wait_timeout(
            &code_integrity_bootstrap_0_ensure_segment_readable_onetime_init_putex,
            3,
            0
    );

    if (putex_rv == 1) {
        LOGD("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) putex signal received.");
    } else {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) putex timeout or failed via putex_result_t(%d)", putex_rv);
        demolisher_smash();
        return;
    }

    pthread_t tid_md5, tid_sha256;
    if (pthread_createM(&tid_md5, NULL, code_integrity_bootstrap_0_md5_thread, NULL)) {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) pthread_create(tid_md5); failed");
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
    }

    if (pthread_detachM(tid_md5)) {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) pthread_detach(tid_md5); failed");
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
    }

    if (pthread_createM(&tid_sha256, NULL, code_integrity_bootstrap_0_sha256_thread, NULL)) {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) pthread_create(tid_sha256); failed");
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
    }

    if (pthread_detachM(tid_sha256)) {
        LOGE("(IntegrityMonitor::CodeIntegrity::Self::Bootstrap0) pthread_detach(tid_sha256); failed");
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
        demolisher_rainbow_terminator();
    }
}
