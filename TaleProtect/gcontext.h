#ifndef TALESTARS_V63_322_GCONTEXT_H
#define TALESTARS_V63_322_GCONTEXT_H

#include <unstdinttypes.h>
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/tools/segment_resolver/segment_resolver.h"
#include <blake3.h>

typedef struct hashset_s {
    u8t sha256[32];
    u8t blake3[BLAKE3_OUT_LEN /* 32b */];
    u8t md5[16];
} hashset_t;

typedef struct keyset_aes_s {
    u8t key[32];
    u8t iv[16];
} keyset_aes_t;

typedef struct lib_hashsets_s {
    hashset_t PT_LOADS[2];
    sizet end_of_exec;
} elf_hashsets_t;

typedef struct code_integrity_s {
    elf_hashsets_t self;
    struct {
        struct {
            elf_hashsets_t tale;
            elf_hashsets_t royale;
            struct {
                segment_t tale;
                segment_t royale;
            } whitelist_ranges;
        } gadgets;
    } external;
} code_integrity_t;

typedef struct integrity_native_lib_dir_entry_s {
    char name[128];
    hashset_t hash;
} integrity_native_lib_dir_entry_t;

typedef struct file_integrity_s {
    integrity_native_lib_dir_entry_t integrity_native_lib_dir_entries[32];
    integrity_native_lib_dir_entry_t entry_not_an_elf;
} file_integrity_t;

//! [integrity_monitor]
typedef struct integrity_monitor_s {
    code_integrity_t code_integrity;
    file_integrity_t file_integrity;
} integrity_monitor_t;

//! [not_an_elf]
typedef struct not_an_elf_s {
    keyset_aes_t keys;
} not_an_elf_t;

//! [maps_monitor]
typedef struct maps_entries_protection_s {
    putex_t putex;
} maps_entries_protection_t;

//! [maps_monitor]
typedef struct maps_monitor_s {
    maps_entries_protection_t maps_entries_protection;
} maps_monitor_t;

//! [process_monitor_s]
typedef struct process_monitor_s {
    maps_monitor_t maps_monitor;
} process_monitor_t;

typedef struct TaleProtectContext_s {
    integrity_monitor_t integrity_monitor;
    process_monitor_t process_monitor;
    not_an_elf_t not_an_elf;
    i8 package_name[64 + 1];
} TaleProtectContext_t;

extern TaleProtectContext_t TaleProtectContext;

void TaleProtectContextInit(void);

#endif //TALESTARS_V63_322_GCONTEXT_H
