//
// Created by johan on 06.10.25.
//
#ifndef TALEROYALE_V63_322_CODE_INTEGRITY_GADGET_ROYALE_TRUSTED_H
#define TALEROYALE_V63_322_CODE_INTEGRITY_GADGET_ROYALE_TRUSTED_H

#include <pthread.h>
#include <unstdinttypes.h>
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/tools/segment_resolver/segment_resolver.h"
#include "TaleProtect/gcontext.h"
#include "TaleProtect/tools/shemaphore/shemaphore.h"

typedef struct {
    putex_t hashchecker_invoker;
    shemaphore_t hashchecker_sem;

    //    putex_t segment_perm_fixer_invoker;
    pthread_once_t ensure_segment_readable_onetime_init_once;
    segments_t libroyale_segments;
    hashset_t trusted_hashset;
} code_integrity_gadget_royale_trusted_ctx_t;

void code_integrity_gadget_royale_trusted(
        void
);

#endif //TALEROYALE_V63_322_CODE_INTEGRITY_GADGET_ROYALE_TRUSTED_H
