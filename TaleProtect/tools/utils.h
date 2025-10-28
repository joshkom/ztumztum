#ifndef TALESTARS_V63_322_UTILS_H
#define TALESTARS_V63_322_UTILS_H

#include <unstdinttypes.h>
#include <global_common/common.h>
#include <TaleProtect/func_resolver/func_resolver_3000.h>

#include <dlfcn.h>

void utils_msleep(
        sizet ms
);

i8 *utils_get_native_libraries_dir(
        void
);

i8 *utils_get_base_apk_install_dir(
        void
);

FORCE_INLINE void *utils_page_align_down_ptr(const void *p, sizet page_size) {
    return (void *) ((uptrt) p & ~(page_size - 1));
}

u8t utils_is_path_suspicious(
        const i8 *path_arg
);

#define BUFFERED_FGETS_SIZE 4096
typedef struct utils_buffered_fgets_ctx_s {
    i32 fd;
    i8 buf[BUFFERED_FGETS_SIZE];
    sizet start;
    sizet end;
} utils_buffered_fgets_ctx_t;

ssizet utils_buffered_fgets(
        utils_buffered_fgets_ctx_t *ctx_arg,
        i8 *dst_arg,
        sizet n_arg
);


#endif //TALESTARS_V63_322_UTILS_H
