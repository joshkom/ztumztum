#ifndef TALESTARS_V63_322_RAINBOW_TERMINATOR_H
#define TALESTARS_V63_322_RAINBOW_TERMINATOR_H

#include <stdlib.h>
#include <setjmp.h>
#include "global_common/common.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "smash.h"
#include "TaleProtect/tools/utils.h"

#include <syscall.h>

#ifndef _RAINBOW_TERMINATOR_ITER
#define _RAINBOW_TERMINATOR_ITER 256
#endif

FORCE_INLINE void _rainbow_terminator_worker_small(i32 id, volatile i32 *sink) {
    volatile i32 buf[8];
    for (i32 i = 0; i < 8; ++i) buf[i] = id ^ (i * 0x9e3779b1);
    for (i32 i = 0; i < 8; ++i) *sink += buf[i];
}

FORCE_INLINE void _rainbow_terminator_worker_medium(i32 id, volatile i32 *sink) {
    volatile i8 tmp[128];
    for (i32 i = 0; i < (i32) sizeof(tmp); ++i) tmp[i] = (i8) (id + i);
    for (i32 i = 0; i < (i32) sizeof(tmp); i += 16) *sink += tmp[i];
}

FORCE_INLINE void _rainbow_terminator_worker_large(i32 id, volatile i32 *sink) {
    volatile i8 big[8192];
    for (i32 i = 0; i < (i32) sizeof(big); i += 31) big[i] = (i8) (id ^ (i & 0xff));
    *sink += big[0];
}

typedef void (*_rainbow_terminator_worker_fn)(i32, volatile i32 *);

FORCE_INLINE void
_rainbow_terminator_trampoline_call(_rainbow_terminator_worker_fn f, i32 id, volatile i32 *sink) {
    if (!f) {
        return;
    }
    f(id, sink);
}

FORCE_INLINE void
_rainbow_terminator_perform_wave(_rainbow_terminator_worker_fn *table, sizet tlen, i32 wave_id,
                                 volatile i32 *sink) {
    u32t r = (u32t) wave_id * 0x9e3779b1u ^ 0xfeedbeefu;
    for (sizet i = 0; i < tlen; ++i) {
        r = (r ^ (r << 13)) ^ (r >> 17) ^ (r << 5);
        sizet idx = (sizet) (r % (u32t) tlen);
        _rainbow_terminator_trampoline_call(table[idx], (i32) (wave_id ^ (i32) idx), sink);
    }
}


FORCE_INLINE void demolisher_rainbow_terminator(void) {
    struct timespec ts;
    secure_syscallM(SYS_clock_gettime, 2, CLOCK_REALTIME, &ts); /* | CLOCK_MONOTONIC */
    u32t seed = (u32t) (ts.tv_sec ^ ts.tv_nsec) ^ (u32t) secure_syscallM(SYS_getpid, 0);
    srand(seed);


    _rainbow_terminator_worker_fn table[16];
    for (i32 i = 0; i < 16; ++i) {
        i32 r = rand() & 3;
        if (r == 0) {
            table[i] = _rainbow_terminator_worker_small;
        } else if (r == 1) {
            table[i] = _rainbow_terminator_worker_medium;
        } else {
            table[i] = _rainbow_terminator_worker_large;
        }
    }

    jmp_buf envA, envB;
    volatile i32 sink = 0;
    i32 stage = 0;

    if (setjmp(envA) == 0) {
        for (i32 wave = 0; wave < (i32) (_RAINBOW_TERMINATOR_ITER / 4); ++wave) {
            _rainbow_terminator_perform_wave(table, 16, wave ^ 0xA5A5, &sink);
        }
        _rainbow_terminator_worker_small(0x11, &sink);
        stage = 1;
        longjmp(envB, 1);
    }

    if (setjmp(envB) == 0) {
        for (i32 wave = 0; wave < (i32) (_RAINBOW_TERMINATOR_ITER / 2); ++wave) {
            _rainbow_terminator_perform_wave(table, 16, wave ^ 0x5A5A, &sink);
            if ((wave & 7) == 3) {
                _rainbow_terminator_worker_medium(wave, &sink);
                longjmp(envA, 1);
            }
        }
    } else {
        for (i32 wave = 0; wave < (i32) (_RAINBOW_TERMINATOR_ITER / 4); ++wave) {
            _rainbow_terminator_perform_wave(table, 16, wave ^ 0xC3C3, &sink);
        }
    }

    const i32 max_depth = 6;
    for (i32 depth = 0; depth < max_depth; ++depth) {
        _rainbow_terminator_worker_large(depth ^ 0x7, &sink);
        _rainbow_terminator_worker_medium(depth ^ 0x3, &sink);
    }

    for (i32 i = 0; i < 64; ++i) {
        sizet idx = (sizet) ((i * 1337u + sink) & 15u);
        _rainbow_terminator_trampoline_call(table[idx], i ^ (i32) sink, &sink);
    }

//    struct timespec ts = {0, 50 * 1000000}; // 50ms
//    nanosleep(&ts, NULL);
    utils_msleep(50);

    demolisher_smash();
}


#endif //TALESTARS_V63_322_RAINBOW_TERMINATOR_H
