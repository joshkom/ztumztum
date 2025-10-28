#ifndef TALESTARS_V63_322_SMASH_H
#define TALESTARS_V63_322_SMASH_H

#include <unstdinttypes.h>
#include <TaleProtect/func_resolver/func_resolver_3000.h>
#include <TaleProtect/secure_syscall/secure_syscall.h>
#include <global_common/common.h>

FORCE_INLINE __attribute__((noreturn)) void demolisher_smash(void) {
//    ERR("[<<<<<<SMASH CALLED>>>>>]");

    u64t target = 0x0;
    volatile u64t trash[512];
    for (int i = 0; i < 512; ++i) {
        trash[i] = 0x0;
    }

    register void *target_addr __asm("x20") = (void *) (target);

    __asm volatile(
            "mov x0, #0x0\n"
            "mov x1, #0x0\n"
            "mov x2, #0x0\n"
            "mov x3, #0x0\n"
            "mov x4, #0x0\n"
            "mov x5, #0x0\n"
            "mov x6, #0x0\n"
            "mov x7, #0x0\n"
            "mov x8, #0x0\n"
            "mov x9, #0x0\n"
            "mov x10, #0x0\n"
            "mov x11, #0x0\n"
            "mov x12, #0x0\n"
            "mov x13, #0x0\n"
            "mov x14, #0x0\n"
            "mov x15, #0x0\n"
            "mov x16, #0x0\n"
            "mov x17, #0x0\n"
            "mov x18, #0x0\n"
            "mov x19, #0x0\n"
            "mov x21, #0x0\n"
            "mov x22, #0x0\n"
            "mov x23, #0x0\n"
            "mov x24, #0x0\n"
            "mov x25, #0x0\n"
            "mov x26, #0x0\n"
            "mov x27, #0x0\n"
            "mov x28, #0x0\n"
            "mov x29, #0x0\n"
            "mov x30, %0\n"
            "mov sp, %0\n"
            "br %0\n"

//            "mov x8, #172\n"
//            "svc #0\n"
//
//            "mov x1, #9\n"
//            "mov x8, #129\n"
//            "svc #0\n"

            :
            : "r"(target_addr)
            : "memory"
            );

    pid_t pid = (pid_t) secure_syscallM(SYS_getpid, 0);
    secure_syscallM(SYS_kill, 2, pid, 9);

    __builtin_unreachable();
}

#endif //TALESTARS_V63_322_SMASH_H
