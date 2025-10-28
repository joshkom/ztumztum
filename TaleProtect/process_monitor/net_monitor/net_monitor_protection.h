#ifndef TALESTARS_V63_322_NET_MONITOR_PROTECTION_H
#define TALESTARS_V63_322_NET_MONITOR_PROTECTION_H

#include <unstdinttypes.h>
#include <TaleProtect/tools/putex/putex.h>

//typedef struct {
//    i32 result;
//    putex_t putex;
//    pthread_once_t main_thread_satisfier_once;
//} net_monitor_loop_ctx_t;

typedef struct {
    putex_t putex;
    atomic_int active_threads;
} net_monitor_runner_ctx_t;

void net_monitor_protection(
        void
);

#endif //TALESTARS_V63_322_NET_MONITOR_PROTECTION_H
