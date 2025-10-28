/*
 * You may think you know what the following code does.
 * But you dont. Trust me.
 * Fiddle with it, and youll spend many a sleepless
 * night cursing the moment you thought youd be clever
 * enough to "optimize" the code below.
 * Now close this file and go play with something else.
 */

#include "im_elfing_it.h"
#include "sections_metadata.h"
#include "aes-whitebox/aes_whitebox.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include <global_common/common.h>
#include <TaleProtect/func_resolver/func_resolver_3000.h>

#include <dlfcn.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>


static void xor_buffer(u8t *buf, sizet len, const u8t key16[16]) {
    sizet i = 0;
    for (; i + 16 <= len; i += 16) {
        buf[i + 0] ^= key16[0];
        buf[i + 1] ^= key16[1];
        buf[i + 2] ^= key16[2];
        buf[i + 3] ^= key16[3];
        buf[i + 4] ^= key16[4];
        buf[i + 5] ^= key16[5];
        buf[i + 6] ^= key16[6];
        buf[i + 7] ^= key16[7];
        buf[i + 8] ^= key16[8];
        buf[i + 9] ^= key16[9];
        buf[i + 10] ^= key16[10];
        buf[i + 11] ^= key16[11];
        buf[i + 12] ^= key16[12];
        buf[i + 13] ^= key16[13];
        buf[i + 14] ^= key16[14];
        buf[i + 15] ^= key16[15];
    }
    for (; i < len; ++i) buf[i] ^= key16[i & 15];
}

static uptrt JOB_ELF_UNPACKED = 0xf7af8eee;

ATTR_USED extern void im_elfing_it(u32t magic, void *pc_arg) {
    LOGW("(UnpackingProcedure::libg::im_elfing_it) unpacker stub called | magic: 0x%x | pc_arg: %p",
         magic, pc_arg);

    if (JOB_ELF_UNPACKED >= 0xF7AF8FCE) {
        return;
    }

    if (magic != 0xbadb00b) {
        return;
    }

    Dl_info info;
    if (dladdrM(pc_arg, &info) == 0) {
        LOGE("(UnpackingProcedure::libg::im_elfing_it) dladdr() failed for pc_arg %p", pc_arg);
        return;
    }

    uptrt base = (uptrt) info.dli_fbase;
    LOGI("(UnpackingProcedure::libg::im_elfing_it) library base: %p (%s)", (void *) base,
         info.dli_fname ? info.dli_fname : "(unknown)");

    int pagesz = getpagesizeM();
    LOGI("(UnpackingProcedure::libg::im_elfing_it) pagesize = %d", pagesz);

    // idx[0] = .rodata | idx[1] = .text
    for (sizet idx = 0; idx < sections_count; ++idx) {
        LOGI("idx->%zu", idx);
        section_metadata_t *sec = &sections[idx];
        const char *sname = (sec->name && sec->name[0])
                            ? sec->name
                            : (idx == 0 ? ".rodata"
                                        : ".text");

        uptrt virt = base + sec->offset;
        u8t *section_ptr = (u8t *) virt;
        sizet size = (sizet) sec->size;

        LOGD("(UnpackingProcedure::libg::im_elfing_it) >>>");
        LOGI("[*] Section %zu/%zu -> %s", idx + 1, sections_count, sname);
        LOGI("    virt_addr: %p", (void *) section_ptr);
        LOGI("    size     : 0x%zx (%zu)", size, size);
        LOGI("    offset   : 0x%zx", (sizet) sec->offset);
        LOGI("    nonce    :");
//        log_nonce(sec->nonce);
        uptrt page_start = (uptrt) section_ptr & ~((uptrt) pagesz - 1);
        uptrt page_end =
                ((uptrt) section_ptr + size + pagesz - 1) & ~((uptrt) pagesz - 1);
        sizet page_len = page_end - page_start;
        LOGI("    page_start: %p, page_len: 0x%zx", (void *) page_start, page_len);

        if (idx == 0) { /* index 0 == rodata => whitebox decrypt */
            u8t *tmp = (u8t *) mallocM(size);
            if (!tmp) {
                LOGE("(UnpackingProcedure::libg::im_elfing_it) malloc(%zu); failed for %s", size,
                     sname);
                continue;
            }

            LOGI("    method: whitebox AES-CTR decrypt (.rodata)");

            if (mprotectM((void *) page_start, page_len, PROT_READ | PROT_WRITE) != 0) {
                LOGE("(UnpackingProcedure::libg::im_elfing_it) mprotectM -> RW failed for %s",
                     sname);
                freeM(tmp);
                continue;
            }

            LOGI("    mprotectM -> RW OK");

            LOGI("encrypted rodata:");
            hexdump(section_ptr, 32);
            unstdmemory_compat_memcpy(tmp, section_ptr, size);
//            memcpy(tmp, section_ptr, size);
//            memcpy(section_ptr, tmp, size);
            aes_whitebox_decrypt_ctr(sec->nonce, tmp, size, section_ptr);

            LOGI("decrypted rodata:");
            hexdump(section_ptr, 32);

            if (mprotectM((void *) page_start, page_len, PROT_READ) != 0) {
                LOGE("(UnpackingProcedure::libg::im_elfing_it) mprotectM restore -> R failed for %s",
                     sname);
            } else {
                LOGI("    restored protection -> R (no X)");
            }

            secure_memory_wipe(tmp, size, SECURE_MEMORY_WIPE_FREE);
            LOGI("    freed tmp for %s", sname);
        } else if (idx == 1) {
            LOGI("    method: XOR with 16-byte repeating nonce (.text)");

//            xor_buffer(tmp, size, sec->nonce);

            if (mprotectM((void *) page_start, page_len, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
                LOGE("[!] mprotectM -> RWX failed for %s", sname);
                continue;
            }
            LOGI("    mprotectM -> RWX OK");

            LOGI("encrypted .text:");
            hexdump(section_ptr, 32);

//            unstdmemory_compat_memcpy(section_ptr, tmp, size);
            xor_buffer(section_ptr, size, sec->nonce);

            LOGI("decrypted .text:");
            hexdump(section_ptr, 32);

            if (mprotectM((void *) page_start, page_len, PROT_READ | PROT_EXEC) != 0) {
                LOGE("(UnpackingProcedure::libg::im_elfing_it) mprotectM restore -> RX failed for %s",
                     sname);
            } else {
                LOGI("    restored protection -> RX");
            }

        } else {
            LOGE("(UnpackingProcedure::libg::im_elfing_it) Unknown index %zu - skipping", idx);
        }

        LOGI("    finished %s", sname);
    }

    JOB_ELF_UNPACKED += 0xe0;
    LOGD("(UnpackingProcedure::libg::im_elfing_it) <<<");
}
