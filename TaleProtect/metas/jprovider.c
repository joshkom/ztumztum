#include "jprovider.h"

JProvider_t jprovider_instance = {
        .magic = 0x9f9904b0f0f00022ULL,
        .vm = NULL,
        .env = NULL,
        .state = -1
};

JProvider_t *meta_jprovider = &jprovider_instance;
