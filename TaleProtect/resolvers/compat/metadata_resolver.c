#include "metadata_resolver.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"

i32 find_metadata_cb(struct dl_phdr_info *info,
                     sizet size,
                     void *data_ptr) {
    find_metadata_ctx_t *ctx = (find_metadata_ctx_t *) data_ptr;

//    LOGI("dlpi_name: %s", info->dlpi_name);

    if (!(info->dlpi_name
          && unstdstring_compat_strstr(info->dlpi_name, ctx->candidate_so_substr))) {
        return 0;
    }

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
             addr + ctx->element_size <= seg_end;
             addr += sizeof(uptrt)) {
            u64t *magic_ptr = (u64t *) addr;
            if (*magic_ptr == ctx->magic) {
                ctx->found = (void *) addr;
                LOGD("Found metadata at %p in %s", (void *) addr, info->dlpi_name);
                return 1;
            }
        }
    }

    return 0;
}

void *find_metadata_by_magic_from_phdr(const i8 *candidate_so_substr_arg,
                                       const u64t magic_value_arg,
                                       sizet element_size_arg) {
    find_metadata_ctx_t ctx;
    ctx.candidate_so_substr = candidate_so_substr_arg;
    ctx.magic = magic_value_arg;
    ctx.found = NULL;
    ctx.element_size = element_size_arg;

    dl_iterate_phdrM(find_metadata_cb, &ctx);

    return ctx.found;
}
