//
// Created by johan on 05.10.25.
//
#ifndef TALESTARS_V63_322_CODE_INTEGRITY_GADGET_TALE_BOOTSTRAP_H
#define TALESTARS_V63_322_CODE_INTEGRITY_GADGET_TALE_BOOTSTRAP_H

#include <pthread.h>
#include <unstdinttypes.h>
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/tools/segment_resolver/segment_resolver.h"

typedef struct {
    putex_t hashchecker_invoker;
//    putex_t segment_perm_fixer_invoker;
    pthread_once_t ensure_segment_readable_onetime_init_once;
    segments_t libtale_segments;
//    hashset_t trusted_hashset;
} code_integrity_gadget_tale_bootstrap_ctx_t;

void code_integrity_gadget_tale_bootstrap(
        void
);

#endif //TALESTARS_V63_322_CODE_INTEGRITY_GADGET_TALE_BOOTSTRAP_H
