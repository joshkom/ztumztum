#ifndef TALESTARS_V63_322_METADATA_HPP
#define TALESTARS_V63_322_METADATA_HPP

#include <unstdinttypes.h>
#include <unstdstring_compat.h>
#include "global_common/common.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"


template<typename T>
T *find_metadata_by_magic_from_phdr(const i8 *candidate_so_substr_arg, const u64t magic_value_arg) {
    struct Context {
        const i8 *candidate_so_substr;
        u64t magic;
        T *found = nullptr;
    } ctx = {candidate_so_substr_arg, magic_value_arg, nullptr};

    auto cb = [](struct dl_phdr_info *info, sizet, void *data_ptr) -> int {
        auto *ctx = reinterpret_cast<Context *>(data_ptr);
        LOGI("dlpi_name: %s", info->dlpi_name);
        if (!(info->dlpi_name && unstdstring_compat_strstr(info->dlpi_name, ctx->candidate_so_substr)))
            return 0;

        uptrt base = info->dlpi_addr;
        const ElfW(Phdr) *phdr = info->dlpi_phdr;
        sizet phnum = info->dlpi_phnum;

        for (sizet i = 0; i < phnum; ++i) {
            if (phdr[i].p_type != PT_LOAD) {
                continue;
            }

            uptrt seg_start = base + phdr[i].p_vaddr;
            uptrt seg_end = seg_start + phdr[i].p_memsz;

            for (uptrt addr = seg_start;
                 addr + sizeof(T) <= seg_end; addr += sizeof(uptrt)) {
                T *candidate = reinterpret_cast<T *>(addr);
                if (candidate->magic == ctx->magic) {
                    ctx->found = candidate;
                    LOGI("Found metadata at %p in %s", (void *) candidate, info->dlpi_name);
                    return 1;
                }
            }
        }
        return 0;
    };

    dl_iterate_phdrM(cb, &ctx);
    return ctx.found;
}


#endif //TALESTARS_V63_322_METADATA_HPP
