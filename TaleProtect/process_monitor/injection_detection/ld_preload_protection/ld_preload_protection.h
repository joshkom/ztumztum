#ifndef TALESTARS_V63_322_LD_PRELOAD_PROTECTION_H
#define TALESTARS_V63_322_LD_PRELOAD_PROTECTION_H

#include "TaleProtect/func_resolver/func_resolver_3000.h"
#include "global_common/common.h"

#include <unstdpair.h>

#define LD_PRELOAD_MAX (512 + 1)

unstdpair_pair_type_namedM(
        i8 ld_preload_value[LD_PRELOAD_MAX],
        u8t result
) injdetection_ld_preload_inspection_t;

injdetection_ld_preload_inspection_t injdetection_ld_preload_inspection(
        void
);

void injdetection_ld_preload_protection(
        void
);

#endif //TALESTARS_V63_322_LD_PRELOAD_PROTECTION_H
