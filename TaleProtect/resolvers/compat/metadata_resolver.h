#ifndef TALESTARS_V63_322_METADATA_RESOLVER_H
#define TALESTARS_V63_322_METADATA_RESOLVER_H

#include <link.h>
#include <stddef.h>
#include <unstdinttypes.h>
#include <unstdstring_compat.h>
#include <global_common/common.h>

// Context structure for the callback
typedef struct find_metadata_ctx_s {
    const i8 *candidate_so_substr;
    void *found;
    u64t magic;
    sizet element_size;
} find_metadata_ctx_t;

// Callback for dl_iterate_phdr
int find_metadata_cb(struct dl_phdr_info *info, size_t size, void *data_ptr);

/**
 * Generic function to find metadata struct by magic field.
 *
 * @param candidate_so_substr_arg substring to match in shared object name
 * @param magic_value_arg value to match in first 8 bytes of struct
 * @param element_size_arg sizeof(struct)
 * @return pointer to found struct, or NULL if not found
 */
void *find_metadata_by_magic_from_phdr(
        const i8 *candidate_so_substr_arg,
        const u64t magic_value_arg,
        sizet element_size_arg
);

#endif //TALESTARS_V63_322_METADATA_RESOLVER_H
