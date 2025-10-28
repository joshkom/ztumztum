#ifndef TALESTARS_V63_322_STACKOVERFLOWER_H
#define TALESTARS_V63_322_STACKOVERFLOWER_H

#include <unstdinttypes.h>
#include <sched.h>
#include <global_common/common.h>

FORCE_INLINE void _stackoverflower_wipe_fp_chain(int max_depth) {
    void **fp = (void **) __builtin_frame_address(0);

    for (int i = 0; i < max_depth && fp != NULL; ++i) {
        if ((uintptr_t) fp < 0x1000) {
            break;
        }
        void **saved_lr_slot = fp + 1;
        volatile void **vslot = (volatile void **) saved_lr_slot;
        *vslot = NULL;
        void *prev_fp = *fp;

        if (prev_fp == NULL || prev_fp == (void *) fp) {
            break;
        }
        fp = (void **) prev_fp;
    }

#if defined(__aarch64__)
    // ARM7 -> fp=x29 | lr=x30
    __asm__ volatile(
            "mov x29, xzr\n\t"
            "mov x30, xzr\n\t"
            :: : // no clobber needed
            );
#elif defined(__arm__)
    // ARM7 -> fp=r11 | lr=r14
    __asm__ volatile(
        "mov r11, #0\n\t"
        "mov lr, #0\n\t"
        ::: // no clobber needed
    );
#endif

}

FORCE_INLINE int demolisher_stackoverflower_inner(sizet depth) {
    volatile char large[8192];
    large[0] = (char) depth;
    if (depth % 16 == 0) {
        _stackoverflower_wipe_fp_chain(32);
        sched_yield();
    }

    if (depth > 1024) {
        _stackoverflower_wipe_fp_chain(64);
    }

    demolisher_stackoverflower_inner(depth + 1);
    (void) large[0];
    return 0;
}

FORCE_INLINE int demolisher_stackoverflower(void) {
    return demolisher_stackoverflower_inner(1);
}

#endif //TALESTARS_V63_322_STACKOVERFLOWER_H
