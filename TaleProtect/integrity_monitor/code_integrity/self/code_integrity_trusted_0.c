#include "code_integrity_trusted_0.h"
#include <TaleProtect/demolisher/rainbow_terminator.h>

#include <TaleProtect/tools/segment_resolver/segment_resolver.h>
#include <TaleProtect/tools/utils.h>
#include <TaleProtect/tools/putex/putex.h>

#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>

pthread_once_t code_integrity_trusted_0_ensure_segment_readable_onetime_init_once = PTHREAD_ONCE_INIT;

void code_integrity_trusted_0_ensure_segment_readable_onetime_init(void) {
    segments_t segs = get_segments((void *) code_integrity_trusted_0_ensure_segment_readable_onetime_init);
    if (!segs.base || !segs.end_of_exec || segs.end_of_exec <= segs.base) {
        LOGE("(CodeIntegrity::Trusted0) invalid segment range — skipping mprotect");
        return;
    }

    sizet page_size = (sizet) sysconfM(_SC_PAGESIZE);
    if (page_size == (sizet) -1) page_size = 4096;

    uptrt start = (uptrt) segs.base;
    uptrt end   = (uptrt) segs.end_of_exec - 1;

    void *mstart = (void *) (start & ~(page_size - 1));
    sizet mlen   = (sizet) ((end - (uptrt) mstart) + 1);

    if (mlen % page_size)
        mlen = ((mlen / page_size) + 1) * page_size;

    if (mprotectM(mstart, mlen, PROT_READ | PROT_EXEC) != 0) {
        LOGE("(CodeIntegrity::Trusted0) ensure_exec_segment_readable_once: mprotect failed");
    } else {
        LOGI("(CodeIntegrity::Trusted0) segment %p..%p is now READ|EXEC (one-time)",
             mstart,
             (void *) ((uptrt) mstart + mlen));
    }
}

_Noreturn void *code_integrity_trusted_0_md5_thread(void *arg) {
    hashset_t *hashes = (hashset_t *) arg;

    segments_t segs = get_segments((void *) code_integrity_trusted_0_md5_thread);
    if (!segs.base || !segs.end_of_exec || segs.end_of_exec <= segs.base) {
        LOGE("(CodeIntegrity::Trusted0::MD5) invalid exec segment range");
        demolisher_rainbow_terminator();
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_md5();

    sizet len = (sizet) (segs.end_of_exec - segs.base);

    while (1) {
        u8t md5_out[16];
        unsigned int out_len = 0;

        EVP_DigestInit_ex(ctx, md, NULL);
        EVP_DigestUpdate(ctx, segs.base, len);
        EVP_DigestFinal_ex(ctx, md5_out, &out_len);

        if (unstdmemory_compat_memcmp(md5_out, hashes->md5, 16) != 0) {
            LOGE("[!] (CodeIntegrity::Trusted0) MD5 hash mismatch detected!");
            LOGE("[*] (CodeIntegrity::Trusted0) correct md5");
            hexdump(md5_out, 16);
            // todo: uncomment before release
             demolisher_rainbow_terminator();
        }

        utils_msleep(440);
    }

    EVP_MD_CTX_free(ctx);
}

_Noreturn void *code_integrity_trusted_0_sha256_thread(void *arg) {
    hashset_t *hashes = (hashset_t *) arg;

    segments_t segs = get_segments((void *) code_integrity_trusted_0_sha256_thread);
    if (!segs.base || !segs.end_of_exec || segs.end_of_exec <= segs.base) {
        LOGE("(CodeIntegrity::Trusted0::SHA256) invalid exec segment range");
        demolisher_rainbow_terminator();
    }

    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

    sizet len = (sizet) (segs.end_of_exec - segs.base);

    while (1) {
        u8t sha_out[32];
        unsigned int out_len = 0;

        EVP_DigestInit_ex(ctx, md, NULL);
        EVP_DigestUpdate(ctx, segs.base, len);
        EVP_DigestFinal_ex(ctx, sha_out, &out_len);

        if (unstdmemory_compat_memcmp(sha_out, hashes->sha256, 32) != 0) {
            LOGE("[!] (CodeIntegrity::Trusted0) SHA256 hash mismatch detected!");
            LOGE("[*] (CodeIntegrity::Trusted0) correct sha256");
            hexdump(sha_out, 32);
            // todo: uncomment before release
             demolisher_rainbow_terminator();
        }

        utils_msleep(720);
//        utils_msleep(100);
    }

    EVP_MD_CTX_free(ctx);
}

void code_integrity_trusted_0(hashset_t *hashes) {
    pthread_onceM(
            &code_integrity_trusted_0_ensure_segment_readable_onetime_init_once,
            code_integrity_trusted_0_ensure_segment_readable_onetime_init
    );

    pthread_t tid_md5, tid_sha256;

    if (pthread_createM(&tid_md5, NULL, code_integrity_trusted_0_md5_thread, (void *) hashes)) {
        demolisher_smash();
    }

    if (pthread_detachM(tid_md5)) {
        demolisher_smash();
    }

    if (pthread_createM(&tid_sha256, NULL, code_integrity_trusted_0_sha256_thread, (void *) hashes)) {
        demolisher_smash();
    }

    if (pthread_detachM(tid_sha256)) {
        demolisher_smash();
    }
}