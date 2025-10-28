#ifndef TALESTARS_V63_322_CODE_INTEGRITY_GADGET_STARS_TRUSTED_CONDITIONAL_H
#define TALESTARS_V63_322_CODE_INTEGRITY_GADGET_STARS_TRUSTED_CONDITIONAL_H

#include <unstdinttypes.h>
#include "global_common/common.h"
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/tools/shemaphore/shemaphore.h"

typedef struct code_integrity_gadget_royale_trusted_conditional_ctx_s {
//    putex_t royale_hashchecker_invoker;
    shemaphore_t royale_hashchecker_sem;
} code_integrity_gadget_royale_trusted_conditional_ctx_t;

EXTERN_C ATTR_VISIBLE ATTR_USED extern void dont_lie_to_me_senpaikun(
        void *void_0_arg,
        void *void_1_arg,
        u32t its_called_motion_arg,
        void *void_3_arg,
        void *void_4_arg,
        void *void_5_arg,
        void *libtale_pc_arg,
        void *libroyale_pc_arg
);

void code_integrity_gadget_royale_trusted_conditional(
        void
);

#endif //TALESTARS_V63_322_CODE_INTEGRITY_GADGET_STARS_TRUSTED_CONDITIONAL_H
