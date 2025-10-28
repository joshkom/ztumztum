#include "secure_syscall.h"
#include "TaleProtect/tools/utils.h"

#include <sys/mman.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>

#include <unstdinttypes.h>
#include <unstdmemory_compat.h>


// ---- prelude for (svc #0 ; ret) ----
secure_syscall_stub_ctx_t secure_syscall_stub_ctx = {
        .encrypted_stub = {
                0x77, 0x77, 0xB9, 0x0C, 0x54, 0xE7, 0x00, 0x15
        },
        .key = {
                0xA3, 0x77, 0xB9, 0x0D, 0x82, 0xB8, 0x03, 0xD5
        }
};

secure_syscall_v2_ctx_t secure_syscall_v2_ctx = {
        {0},
        PTHREAD_ONCE_INIT
};

typedef struct secure_syscall_v2_stubs_monitor_ctx_s {
    const u8t *encrypted;
    const u8t *key;
} secure_syscall_v2_stubs_monitor_ctx_t;

_Noreturn static void *secure_syscall_v2_stubs_monitor(void *arg) {
    secure_syscall_v2_stubs_monitor_ctx_t *stubs_monitor_ctx = (secure_syscall_v2_stubs_monitor_ctx_t *) arg;

    u8t expected_stub[_SECURE_SYSCALL_STUB_SIZE_BYTES];
    for (i32 word = 0; word < 2; ++word) {
        for (i32 b = 0; b < 4; ++b) {
            sizet src_idx = word * 4 + (3 - b);
            expected_stub[word * 4 + b] =
                    secure_syscall_stub_ctx.encrypted_stub[src_idx] ^ secure_syscall_stub_ctx.key[src_idx];
//                    stubs_monitor_ctx->encrypted[src_idx] ^ stubs_monitor_ctx->key[src_idx]; /* weirdly enough, doesn't work */
        }
    }

    LOGI("(SecureSyscallV2::Monitor) stubs monitor initiated");

//    LOGE("expected stub:");
//    hexdump(expected_stub, _SECURE_SYSCALL_STUB_SIZE_BYTES);
//
//    LOGE("stubs[0..18]:");
//    for (int i = 0; i < _SECURE_SYSCALL_V2_MAX_STUBS; ++i) {
//        hexdump(secure_syscall_v2_ctx.stubs[i], _SECURE_SYSCALL_STUB_SIZE_BYTES);
//    }

    while (1) {
        for (sizet i = 0; i < _SECURE_SYSCALL_V2_MAX_STUBS; i++) {
            u8t *stub = secure_syscall_v2_ctx.stubs[i];
            if (!stub) {
                continue;
            }

            for (sizet b = 0; b < _SECURE_SYSCALL_STUB_SIZE_BYTES; b++) {
                if (stub[b] != expected_stub[b]) {
                    LOGE("stub[%d](0x%zx) != expected_stub[%d](0x%zx)", stub[b], b, expected_stub[b], b);
                    LOGE("(SecureSyscallV2::Monitor) tampered stub detected. aborting...");
                    demolisher_stackoverflower();
                }
            }
        }

        utils_msleep(680);
    }
}

void secure_syscall_v2_onetime_stubs_init(void) {
    srand((u32t) time(NULL));

    for (i32 i = 0; i < _SECURE_SYSCALL_V2_MAX_STUBS; i++) {
        u8t *stub_addr = (u8t *) mmapM(
                NULL, 8,
                PROT_READ | PROT_WRITE,
                MAP_ANON | MAP_PRIVATE,
                -1, 0
        );

        if (stub_addr == MAP_FAILED) {
            stub_addr = NULL;
            demolisher_stackoverflower();
            continue;
        }

        for (i32 word = 0; word < 2; ++word) {
            for (i32 b = 0; b < 4; ++b) {
                sizet src_idx = word * 4 + (3 - b);
                stub_addr[word * 4 + b] =
                        secure_syscall_stub_ctx.encrypted_stub[src_idx] ^
                        secure_syscall_stub_ctx.key[src_idx];
            }
        }

        __builtin___clear_cache((i8 *) stub_addr, (i8 *) (stub_addr + 8));
        if (mprotectM(stub_addr, 8, PROT_READ | PROT_EXEC) != 0) {
            munmapM(stub_addr, 8);
            stub_addr = NULL;
            demolisher_stackoverflower();
        }

        secure_syscall_v2_ctx.stubs[i] = stub_addr;
    }

    LOGI("(SecureSyscallV2) stubs initiated");

    secure_syscall_v2_stubs_monitor_ctx_t monitor_arg = {
            .encrypted = secure_syscall_stub_ctx.encrypted_stub,
            .key       = secure_syscall_stub_ctx.key
    };

    pthread_t secure_syscall_v2_stubs_monitor_tid;
    if (pthread_createM(&secure_syscall_v2_stubs_monitor_tid,
                        NULL,
                        secure_syscall_v2_stubs_monitor,
                        &monitor_arg)) {
        demolisher_stackoverflower();
    }

    if (pthread_detachM(secure_syscall_v2_stubs_monitor_tid)) {
        demolisher_stackoverflower();
    }
}


secure_syscall_lite_ctx_t secure_syscall_lite_ctx = {
        NULL,
        PTHREAD_ONCE_INIT
};

void secure_syscall_lite_onetime_stub_init(void) {
    secure_syscall_lite_ctx.stub_addr = (u8t *) mmapM(
            NULL, 8,
            PROT_READ | PROT_WRITE,
            MAP_ANON | MAP_PRIVATE,
            -1, 0
    );
    if (secure_syscall_lite_ctx.stub_addr == MAP_FAILED) {
        secure_syscall_lite_ctx.stub_addr = NULL;
        demolisher_stackoverflower();
        return;
    }

    for (i32 word = 0; word < 2; ++word) {
        for (i32 b = 0; b < 4; ++b) {
            sizet src_idx = word * 4 + (3 - b);
            secure_syscall_lite_ctx.stub_addr[word * 4 + b] =
                    secure_syscall_stub_ctx.encrypted_stub[src_idx] ^
                    secure_syscall_stub_ctx.key[src_idx];
        }
    }

    __builtin___clear_cache((i8 *) secure_syscall_lite_ctx.stub_addr,
                            (i8 *) (secure_syscall_lite_ctx.stub_addr + 8));
    if (mprotectM(secure_syscall_lite_ctx.stub_addr, 8, PROT_READ | PROT_EXEC) != 0) {
        demolisher_stackoverflower();
        munmapM(secure_syscall_lite_ctx.stub_addr, 8);
        secure_syscall_lite_ctx.stub_addr = NULL;
    }

    LOGI("[SecureSyscall::lite] stub initiated");
}

uptrt secure_syscall_lite(i32 svc_num_arg, i32 nargs_arg, ...) {
    svc_num_arg = DEOBF_INT32(svc_num_arg);
    nargs_arg = DEOBF_INT32(nargs_arg);

    if (nargs_arg > 6) {
        LOGE("(SecureSyscall::lite) args limit of 6 exceeded = %d", nargs_arg);
        return -1;
    }

    pthread_once(&secure_syscall_lite_ctx.stub_init, secure_syscall_lite_onetime_stub_init);
    if (!secure_syscall_lite_ctx.stub_addr) {
        LOGE("(SecureSyscall::lite) pthread_once failed.");
        return -1;
    }

    uptrt args[6] = {0};
    va_list vl;
    va_start(vl, nargs_arg);
    for (i32 i = 0; i < nargs_arg; i++) {
        args[i] = va_arg(vl, uptrt);
    }
    va_end(vl);

    uptrt retval;
    __asm volatile(
            "mov x0, %1\n"
            "mov x1, %2\n"
            "mov x2, %3\n"
            "mov x3, %4\n"
            "mov x4, %5\n"
            "mov x5, %6\n"

            "mov w9, %w7\n"

            "ldr x11, =0xA5C3F1D7\n"
            "eor x10, x9, x11\n"
            "eor x10, x10, x11\n"

            "ror x10, x10, #19\n"
            "ror x10, x10, #45\n"

            "lsl x10, x10, #7\n"
            "lsr x10, x10, #7\n"

            "ldr x12, =0x37A2B1C4\n"
            "add x10, x10, x12\n"
            "sub x10, x10, x12\n"

            "mov x8, x10\n"

            "blr %8\n"
            "mov %0, x0\n"
            : "=r"(retval)
            :
            "r"(args[0]), "r"(args[1]), "r"(args[2]),
            "r"(args[3]), "r"(args[4]), "r"(args[5]),
            "r"(svc_num_arg), "r"(secure_syscall_lite_ctx.stub_addr)
            : "x0", "x1", "x2", "x3", "x4", "x5", "x8", "x9", "x10", "x11", "x12"
            );

    return retval;
}

uptrt secure_syscall_v2(i32 svc_num_arg, i32 nargs_arg, ...) {
    svc_num_arg = DEOBF_INT32(svc_num_arg);
    nargs_arg = DEOBF_INT32(nargs_arg);

    if (nargs_arg > 6) {
        return -1;
    }

    pthread_once(&secure_syscall_v2_ctx.stub_init, secure_syscall_v2_onetime_stubs_init);

    u8t *stub_addr = secure_syscall_v2_ctx.stubs[rand() % _SECURE_SYSCALL_V2_MAX_STUBS];
    if (!stub_addr) {
        return -1;
    }

    uptrt args[6] = {0};
    va_list vl;
    va_start(vl, nargs_arg);
    for (i32 i = 0; i < nargs_arg; i++) {
        args[i] = va_arg(vl, uptrt);
    }
    va_end(vl);

    uptrt retval;
    __asm volatile(
            "mov x0, %1\n"
            "mov x1, %2\n"
            "mov x2, %3\n"
            "mov x3, %4\n"
            "mov x4, %5\n"
            "mov x5, %6\n"

            "mov w9, %w7\n"

            "ldr x11, =0xA5C3F1D7\n"
            "eor x10, x9, x11\n"
            "eor x10, x10, x11\n"

            "ror x10, x10, #19\n"
            "ror x10, x10, #45\n"

            "lsl x10, x10, #7\n"
            "lsr x10, x10, #7\n"

            "ldr x12, =0x37A2B1C4\n"
            "add x10, x10, x12\n"
            "sub x10, x10, x12\n"

            "mov x8, x10\n"

            "blr %8\n"
            "mov %0, x0\n"
            : "=r"(retval)
            :
            "r"(args[0]), "r"(args[1]), "r"(args[2]),
            "r"(args[3]), "r"(args[4]), "r"(args[5]),
            "r"(svc_num_arg), "r"(stub_addr)
            : "x0", "x1", "x2", "x3", "x4", "x5", "x8", "x9", "x10", "x11", "x12"
            );

    return retval;
}
