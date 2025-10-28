#ifndef TALESTARS_V63_322_PAGE_MOLESTER_H
#define TALESTARS_V63_322_PAGE_MOLESTER_H

#include <unstdinttypes.h>
#include <bits/sysconf.h>
#include "global_common/common.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"

FORCE_INLINE void *page_align_down(void *addr, sizet page_size) {
    return (void *) ((uptrt) addr & ~((uptrt) page_size - 1));
}

/* Set the single page that contains `addr` to PROT_READ (read-only).
 * Returns 0 on success, -1 on failure and errno is set.
 */
FORCE_INLINE int demolisher_page_molester_set_page_readonly(void *addr_arg) {
    long ps = sysconfM(_SC_PAGESIZE);
    if (ps <= 0) {
        return -1;
    }
    size_t page_size = (sizet) ps;

    void *base = page_align_down(addr_arg, page_size);

    if (mprotectM(base, page_size, PROT_NONE) != 0) {
        LOGE("mprotect(PROT_READ) failed");
        return -1;
    }

    return 0;
}

#endif //TALESTARS_V63_322_PAGE_MOLESTER_H
