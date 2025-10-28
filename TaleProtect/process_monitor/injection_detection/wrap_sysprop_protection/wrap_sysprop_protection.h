#ifndef TALESTARS_V63_322_WRAP_SYSPROP_PROTECTION_H
#define TALESTARS_V63_322_WRAP_SYSPROP_PROTECTION_H

#include <unstdinttypes.h>
#include <unstdpair.h>
#include <sys/system_properties.h>

unstdpair_pair_type_named_legacyM(
        i8 wrap_value[PROP_VALUE_MAX + 1];
        u8t result;
) injdetection_wrap_sysprop_inspection_t;

injdetection_wrap_sysprop_inspection_t injdetection_wrap_sysprop_inspection(
        void
);

void injdetection_wrap_sysprop_protection(
        void
);

#endif //TALESTARS_V63_322_WRAP_SYSPROP_PROTECTION_H
