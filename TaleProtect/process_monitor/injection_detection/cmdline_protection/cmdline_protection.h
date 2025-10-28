#ifndef TALESTARS_V63_322_CMDLINE_PROTECTION_H
#define TALESTARS_V63_322_CMDLINE_PROTECTION_H

#include "global_common/common.h"

typedef struct {
    u8t result;
    i8 cmdline[4096];
    i8 parent_cmdline[4096];
} injdetection_cmdline_inspection_t;

injdetection_cmdline_inspection_t injdetection_cmdline_inspection(
        void
);

void injdetection_cmdline_protection(
        void
);

#endif //TALESTARS_V63_322_CMDLINE_PROTECTION_H
