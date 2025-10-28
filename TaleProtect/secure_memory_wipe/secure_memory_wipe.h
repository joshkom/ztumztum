#ifndef TALESTARS_V63_322_SECURE_MEMORY_WIPE_H
#define TALESTARS_V63_322_SECURE_MEMORY_WIPE_H


#include "global_common/common.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"

#define SECURE_MEMORY_WIPE_FREE 1
#define SECURE_MEMORY_WIPE_KEEP 0

FORCE_INLINE void secure_memory_wipe(void *buffer_arg, sizet size_arg, u8t free_flag) {
    if (!buffer_arg || size_arg == 0)
        return;

    unsigned char *start = (unsigned char *) buffer_arg;
    unsigned char *end = start + size_arg;

    // [align:2]
    if (((uptrt) start & 1) && start < end) {
        *start++ = 0;
    }
    if (((uptrt) end & 1) && start < end) {
        *(--end) = 0;
    }

    // [align:4]
    if (((uptrt) start & 2) && (end - start) >= 2) {
        *(u16t *) start = 0;
        start += 2;
    }
    if (((uptrt) end & 2) && (end - start) >= 2) {
        end -= 2;
        *(u16t *) end = 0;
    }

    // [align:8]
    if (((uptrt) start & 4) && (end - start) >= 4) {
        *(u32t *) start = 0;
        start += 4;
    }
    if (((uptrt) end & 4) && (end - start) >= 4) {
        end -= 4;
        *(u32t *) end = 0;
    }

    u64t *p64 = (u64t *) start;
    u64t *p64_end = (u64t *) end;
    while (p64 < p64_end) {
        *p64++ = 0ULL;
    }

    while ((unsigned char *) p64 < end) {
        *((unsigned char *) p64) = 0;
        p64 = (u64t *) ((unsigned char *) p64 + 1);
    }

    if (free_flag == SECURE_MEMORY_WIPE_FREE) {
        freeM(buffer_arg);
    }
}

#endif //TALESTARS_V63_322_SECURE_MEMORY_WIPE_H
