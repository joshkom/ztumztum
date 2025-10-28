#include "not_an_elf.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include <unstdstring_compat.h>

static inline __attribute__((always_inline)) unsigned char *decode_libtale_not_an_elf_so__22(void) {
    size_t __hc_len = 23u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x185D189A5B0BD95BULL;
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
    __hc_tmpB = 0xD355A51AE3FFFCE0ULL;
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
    __hc_tmpB = 0x1940001BBF696C55ULL;
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
    buf[22] = 0x0;
    return buf;
}

void not_an_elf_decrypt_config(void) {
    LOGD("(NotAnElf) decrypting libtale.not_an_elf.so...");
    NotAnElf_t instance_not_an_elf;
    AES256Encryptor aes256enc;
    aes256_init(
            &aes256enc,
            TaleProtectContext.not_an_elf.keys.key,
            sizeof(TaleProtectContext.not_an_elf.keys.key)
    );

    i8 *decoded_libtale_not_an_elf_so = (i8 *) decode_libtale_not_an_elf_so__22();
    i8 *not_an_elf_path = unstdstring_compat_strcat(
            utils_get_native_libraries_dir(),
            decoded_libtale_not_an_elf_so
    );

    LOGI("(NotAnElf) path: \"%s\"", not_an_elf_path);
    aes256_decrypt_and_deserialize_from_file(
            &aes256enc,
            not_an_elf_path,
            ((ubytes) {.length = sizeof(TaleProtectContext.not_an_elf.keys.iv), .capacity = 0, .data = TaleProtectContext.not_an_elf.keys.iv,}),
            &instance_not_an_elf,
            sizeof(NotAnElf_t)
    );
    secure_memory_wipe(
            decoded_libtale_not_an_elf_so,
            22,
            SECURE_MEMORY_WIPE_FREE
    );
    secure_memory_wipe(
            not_an_elf_path,
            unstdstring_compat_strlen8(not_an_elf_path),
            SECURE_MEMORY_WIPE_FREE
    );

    LOGD("(NotAnElf) libtale.not_an_elf.so decrypted successfully.");

    LOGD("(NotAnElf) setting integrity file entries...");
    for (sizet i = 0; *instance_not_an_elf.integrity_native_lib_dir_entries[i].name; ++i) {
        unstdmemory_compat_memcpy(
                TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].hash.blake3,
                instance_not_an_elf.integrity_native_lib_dir_entries[i].hash.blake3,
                sizeof(TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].hash.blake3)
        );
        unstdmemory_compat_memcpy(
                TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].name,
                instance_not_an_elf.integrity_native_lib_dir_entries[i].name,
                sizeof(TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].name)
        );

        LOGI("  Entry[%zu (%s)].blake3->", i,
             TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].name);
        hexdump(TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[i].hash.blake3,
                32);
    }

    for (sizet i = 0; *instance_not_an_elf.integrity_native_lib_dir_entries[i].name; ++i) {
        secure_memory_wipe(
                &instance_not_an_elf.integrity_native_lib_dir_entries[i],
                sizeof(integrity_native_lib_dir_entry_t),
                SECURE_MEMORY_WIPE_KEEP
        );
    }

    u8t not_an_elf_hash[] = {
            0x75, 0x87, 0x57, 0x66, 0xfe, 0xd7, 0x23, 0xbd,
            0xa4, 0x5e, 0xd5, 0x09, 0x90, 0x16, 0xc4, 0xd2,
            0x86, 0x68, 0x21, 0xfc, 0x4a, 0xb9, 0x09, 0xf8,
            0x1e, 0xc7, 0xb4, 0xcb, 0xc5, 0x3a, 0x8a, 0xf5
    };

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.file_integrity.entry_not_an_elf.hash.blake3,
            not_an_elf_hash,
            sizeof(not_an_elf_hash)
    );

    //! external::gadgets::tale
//    TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.end_of_exec = instance_not_an_elf.gadgets.tale.gadget.end_of_exec;
    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].sha256,
            instance_not_an_elf.gadgets.tale.gadget.PT_LOADS[0].sha256,
            32
    );

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].blake3,
            instance_not_an_elf.gadgets.tale.gadget.PT_LOADS[0].blake3,
            32
    );

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].md5,
            instance_not_an_elf.gadgets.tale.gadget.PT_LOADS[0].md5,
            16
    );

    //! external::gadgets::royale
//    TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.end_of_exec = instance_not_an_elf.gadgets.royale.gadget.end_of_exec;
    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].sha256,
            instance_not_an_elf.gadgets.stars.gadget.PT_LOADS[0].sha256,
            32
    );

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].blake3,
            instance_not_an_elf.gadgets.stars.gadget.PT_LOADS[0].blake3,
            32
    );

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].md5,
            instance_not_an_elf.gadgets.stars.gadget.PT_LOADS[0].md5,
            16
    );

    //! self::hashset
    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0].sha256,
            instance_not_an_elf.taleprotect.PT_LOADS[0].sha256,
            32
    );

    unstdmemory_compat_memcpy(
            TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0].md5,
            instance_not_an_elf.taleprotect.PT_LOADS[0].md5,
            16
    );

    LOGI("(NotAnElf) [integrity_monitor::code_monitor::self]");
    LOGI("  self::PT[0].sha256->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0].sha256, 32);
    LOGI("  self::PT[0].blake3->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0].blake3, 32);
    LOGI("  self::PT[0].md5->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0].md5, 16);


    LOGI("(NotAnElf) [integrity_monitor::code_monitor::external::gadgets::tale]");
    LOGI("  end_of_exec (save) -> 0x%zx",
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.end_of_exec);
    LOGI("  end_of_exec (conf) -> 0x%zx", instance_not_an_elf.gadgets.tale.gadget.end_of_exec);
    LOGI("  tale::PT[0].sha256->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].sha256,
            32);
    LOGI("  tale::PT[0].blake3->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].blake3,
            32);
    LOGI("  tale::PT[0].md5->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.tale.PT_LOADS[0].md5,
            16);

    //! todo: comment these
    LOGI("(NotAnElf) [integrity_monitor::code_monitor::external::gadgets::royale]");
    LOGI("  end_of_exec (save) -> 0x%zx",
         TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.end_of_exec);
    LOGI("  end_of_exec (conf) -> 0x%zx", instance_not_an_elf.gadgets.stars.gadget.end_of_exec);
    LOGI("  royale::PT[0].sha256->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].sha256,
            32);
    LOGI("  royale::PT[0].blake3->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].blake3,
            32);
    LOGI("  royale::PT[0].md5->");
    hexdump(TaleProtectContext.integrity_monitor.code_integrity.external.gadgets.royale.PT_LOADS[0].md5,
            16);

    secure_memory_wipe(
            &instance_not_an_elf,
            sizeof(NotAnElf_t),
            SECURE_MEMORY_WIPE_KEEP
    );

    secure_memory_wipe(
            &aes256enc,
            sizeof(AES256Encryptor),
            SECURE_MEMORY_WIPE_KEEP
    );
}
