//
// Created by johan on 10.10.25.
//
#include <unstdstring_compat.h>
#include <unstdprintf_compat.h>
#include <errno.h>
#include <fcntl.h>
#include "native_library_directory_file_integrity.h"

#include "TaleProtect/tools/list_dir_entries/list_dir_entries.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/gcontext.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/demolisher/stackoverflower.h"


#include "blake3.h"

#define HASH_READ_LIMIT (50 * 1024)
#define READ_CHUNK 4096

static inline __attribute__((always_inline)) unsigned char *decode__s__s__5(void) {
    size_t __hc_len = 6u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0x001CC94BDCC94000ULL;
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

static inline __attribute__((always_inline)) unsigned char *decode_libtale_not_an_elf_so__21(void) {
    size_t __hc_len = 22u;
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
    __hc_tmpB = 0xE964AA24CF08FC20ULL;
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
    __hc_tmpB = 0x9B000000787E3E57ULL;
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
    buf[21] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_libtale_protect_so__18(void) {
    size_t __hc_len = 19u;
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
    __hc_tmpB = 0xA24FA720E3FFE122ULL;
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
    __hc_tmpB = 0xDCC0000063A2F5D5ULL;
    __hc_tmpB = (((((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) << 2) |
                  (((((__hc_tmpB ^ 0x0000000063A2F5CEULL) >> 56) |
                     ((__hc_tmpB ^ 0x0000000063A2F5CEULL) << 8)) & 0xFFFFFFFFFFFFFFFFULL) >> 62)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[16] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[17] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[18] = 0x0;
    return buf;
}

u8t native_library_directory_entries_integrity_inspection(void) {
    i8 *native_lib_dir_path = utils_get_native_libraries_dir();
    list_dir_entries_alloced_result_t native_lib_dir_entries = list_dir_entries_alloced(
            native_lib_dir_path);

    i8 *decoded__s__s = (i8 *) decode__s__s__5();

    i8 *decoded_libtale_protect_so = (i8 *) decode_libtale_protect_so__18();
    i8 *decoded_libtale_not_an_elf_so = (i8 *) decode_libtale_not_an_elf_so__21();

    for (u32t i = 0; i < native_lib_dir_entries.count; ++i) {
        const i8 *entry_name = native_lib_dir_entries.entries[i].name;

        if (native_lib_dir_entries.entries[i].type != LDE_ENTRY_TYPE_FILE) {
            LOGE("(Integrity::NativeLib) abnormal entry type found: \"%s\"", entry_name);
            demolisher_stackoverflower();
            return 1;
        }

        char full_path[512];
        unstdprintf_snprintf(
                full_path,
                sizeof(full_path),
                decoded__s__s,
                native_lib_dir_path,
                entry_name
        );

        i32 fd = (int) secure_syscall_v2M(SYS_openat, 4, AT_FDCWD, full_path, O_RDONLY, 0);
        if (fd < 0) {
            LOGW("(Integrity::NativeLib) openat failed for \"%s\": %s", full_path, strerror(errno));
            demolisher_stackoverflower();
            return 1;
        }

        if (secure_syscall_v2M(SYS_lseek, 3, fd, 0L, SEEK_SET) < 0) {
            LOGW("(Integrity::NativeLib) lseek failed for \"%s\": %s", full_path, strerror(errno));
            secure_syscall_v2M(SYS_close, 1, fd);
            continue;
        }

        blake3_hasher hasher;
        blake3_hasher_init(&hasher);

        u8 buf[READ_CHUNK];
        sizet total = 0;

        while (total < HASH_READ_LIMIT) {
            ssize_t r = (ssize_t) secure_syscall_v2M(SYS_read, 3, fd, buf, READ_CHUNK);
            if (r <= 0) break;

            sizet to_hash = (total + r > HASH_READ_LIMIT) ? (HASH_READ_LIMIT - total) : (sizet) r;
            blake3_hasher_update(&hasher, buf, to_hash);
            total += to_hash;
        }

        secure_syscall_v2M(SYS_close, 1, fd);

        u8 hash[BLAKE3_OUT_LEN];
        blake3_hasher_finalize(&hasher, hash, sizeof(hash));

        LOGD("(Integrity::NativeLib) file=\"%s\" hash:", entry_name);
        hexdump(hash, BLAKE3_OUT_LEN);


        // todo: implement self on-disk hash checking later and fix this chicken-egg problem.
        if (!unstdstring_compat_strncmp(entry_name, decoded_libtale_protect_so, 18)
            && unstdstring_compat_strlen8(entry_name) == 18) {
            LOGW("(Integrity::NativeLib) skipping self \"libtale.protect.so\"");
            continue;
        }

        if (!unstdstring_compat_strncmp(entry_name, decoded_libtale_not_an_elf_so, 21)
            && unstdstring_compat_strlen8(entry_name) == 21) {
            LOGW("(Integrity::NativeLib) skipping config \"libtale.not_an_elf.so\"");
            continue;
            if (unstdmemory_compat_memcmp(
                    TaleProtectContext.integrity_monitor.file_integrity.entry_not_an_elf.hash.blake3,
                    hash,
                    BLAKE3_OUT_LEN)) {
                LOGE("(Integrity::NativeLib) special config file \"libtale.not_an_elf.so\" hash mismatch!");
                list_dir_entries_free(&native_lib_dir_entries);
                return 1;
            } else {
                LOGI("(Integrity::NativeLib) special config file \"libtale.not_an_elf.so\" hash OK");
            }
            continue;
        }

        u32t table_len =
                sizeof(TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries) /
                sizeof(TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[0]);
        u8t found = 0;

        for (u32t j = 0; j < table_len; ++j) {
            const char *expected_name = TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[j].name;
            sizet expected_len = unstdstring_compat_strlen8(expected_name);

            if (unstdstring_compat_strlen8(entry_name) == expected_len &&
                !unstdstring_compat_strncmp(entry_name, expected_name, expected_len)) {

                found = 1;

                if (unstdmemory_compat_memcmp(
                        TaleProtectContext.integrity_monitor.file_integrity.integrity_native_lib_dir_entries[j].hash.blake3,
                        hash,
                        BLAKE3_OUT_LEN)) {
                    LOGE("(Integrity::NativeLib) file \"%s\" hash mismatch!", entry_name);
                    list_dir_entries_free(&native_lib_dir_entries);
                    return 1;
                } else {
                    LOGI("(Integrity::NativeLib) file \"%s\" hash OK", entry_name);
                }
                break;
            }
        }

        if (!found) {
            LOGE("(Integrity::NativeLib) file \"%s\" not found in integrity table!", entry_name);
            list_dir_entries_free(&native_lib_dir_entries);
            return 1;
        }
    }

    secure_memory_wipe(decoded_libtale_protect_so, 18, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_libtale_not_an_elf_so, 21, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded__s__s, 5, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(
            native_lib_dir_path,
            unstdstring_compat_strlen8(native_lib_dir_path),
            SECURE_MEMORY_WIPE_KEEP
    );

    list_dir_entries_free(&native_lib_dir_entries);

    return 0;
}

void *native_library_directory_entries_integrity_mitigation(void *arg) {
    (void) arg;
    if (native_library_directory_entries_integrity_inspection()) {
        demolisher_smash();
    }
    return NULL;
}

void native_library_directory_entries_integrity_protection(void) {
    pthread_t th0;
    if (pthread_createM(&th0, NULL,
                        native_library_directory_entries_integrity_mitigation,
                        NULL)) {
        demolisher_smash();
    }
}
