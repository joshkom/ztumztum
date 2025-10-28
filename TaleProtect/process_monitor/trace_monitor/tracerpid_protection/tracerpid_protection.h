#ifndef TALESTARS_V63_322_TRACERPID_PROTECTION_H
#define TALESTARS_V63_322_TRACERPID_PROTECTION_H

#include <unstdinttypes.h>
#include "TaleProtect/tools/putex/putex.h"

typedef struct tracerpid_protection_loop_ctx_s {
    putex_t putex;
//    pthread_once_t main_thread_satisfier_once;
    int result;
} tracerpid_protection_loop_ctx_t;

void tracerpid_protection(
        void
);

#endif //TALESTARS_V63_322_TRACERPID_PROTECTION_H
