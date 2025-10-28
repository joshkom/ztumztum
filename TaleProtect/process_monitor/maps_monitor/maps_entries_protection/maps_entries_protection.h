#ifndef TALESTARS_V63_322_MAPS_ENTRIES_PROTECTION_H
#define TALESTARS_V63_322_MAPS_ENTRIES_PROTECTION_H

#include <unstdinttypes.h>
#include "TaleProtect/tools/putex/putex.h"

typedef struct {
    i32 result;
    putex_t putex;
    pthread_once_t main_thread_satisfier_once;
} maps_monitor_maps_entries_loop_ctx_t;

void maps_monitor_maps_entries_protection(
        void
);

#endif //TALESTARS_V63_322_MAPS_ENTRIES_PROTECTION_H
