#ifndef TALESTARS_V63_322_NOT_AN_ELF_H
#define TALESTARS_V63_322_NOT_AN_ELF_H

#include "TaleProtect/gcontext.h"
#include "TaleProtect/talecrypt/compat/talecrypt.h"

//#pragma pack(push, 1) /* packing alignment = 1 */

typedef struct __attribute__((__packed__)) NotAnElf_s {
    elf_hashsets_t taleprotect;
    struct {
        struct {
            elf_hashsets_t gadget;
            hashset_t config;
            hashset_t script;
        } tale;
        struct {
            elf_hashsets_t gadget;
            hashset_t config;
            hashset_t script;
        } stars;
    } gadgets;
    integrity_native_lib_dir_entry_t integrity_native_lib_dir_entries[32];
} NotAnElf_t;

// v1
//typedef struct __attribute__((__packed__)) NotAnElf_s {
//    elf_hashsets_t taleprotect;
//    struct {
//        elf_hashsets_t tale;
//        elf_hashsets_t royale;
//    } gadgets;
//} NotAnElf_t;

//#pragma pack(pop) /* restore previous alignment */

void not_an_elf_decrypt_config(void);

#endif //TALESTARS_V63_322_NOT_AN_ELF_H
