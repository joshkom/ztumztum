#ifndef TALESTARS_V63_322_SECURE_SYSCALL_H
#define TALESTARS_V63_322_SECURE_SYSCALL_H

#include <unstdinttypes.h>
#include <global_common/common.h>

#include <sys/mman.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <syscall.h>

#include <unstdinttypes.h>
#include <unstdmemory_compat.h>
#include <TaleProtect/func_resolver/func_resolver_3000.h>
#include <TaleProtect/obfuscators/literal_obfuscator.h>
#include <TaleProtect/demolisher/stackoverflower.h>
#include <threads.h>

// ---- prelude for (svc #0 ; ret) ----
#define _SECURE_SYSCALL_STUB_SIZE_BYTES 8
#define _SECURE_SYSCALL_KEY_SIZE_BYTES 8
typedef struct secure_syscall_stub_ctx_s {
    u8t encrypted_stub[_SECURE_SYSCALL_STUB_SIZE_BYTES];
    u8t key[_SECURE_SYSCALL_KEY_SIZE_BYTES];
} secure_syscall_stub_ctx_t;
extern secure_syscall_stub_ctx_t secure_syscall_stub_ctx;

#define secure_syscallM(svc_num_arg, nargs_arg, ...) secure_syscall_v2(OBF_INT32(svc_num_arg), OBF_INT32(nargs_arg), ##__VA_ARGS__)

// ================== [secure syscall :: v2] ==================
#define _SECURE_SYSCALL_V2_MAX_STUBS 18
typedef struct secure_syscall_v2_ctx_s {
    u8t *stubs[_SECURE_SYSCALL_V2_MAX_STUBS];
    pthread_once_t stub_init;
} secure_syscall_v2_ctx_t;
extern secure_syscall_v2_ctx_t secure_syscall_v2_ctx;

uptrt secure_syscall_v2(i32 svc_num_arg, i32 nargs_arg, ...);

#define secure_syscall_v2M(svc_num_arg, nargs_arg, ...) secure_syscall_v2(OBF_INT32(svc_num_arg), OBF_INT32(nargs_arg), ##__VA_ARGS__)

// ================== [secure syscall :: lite] ==================
typedef struct secure_syscall_lite_ctx_s {
    u8t *stub_addr;
    pthread_once_t stub_init;
} secure_syscall_lite_ctx_t;
extern secure_syscall_lite_ctx_t secure_syscall_lite_ctx;

uptrt secure_syscall_lite(i32 svc_num_arg, i32 nargs_arg, ...);

#define secure_syscall_liteM(svc_num_arg, nargs_arg, ...) secure_syscall_lite(OBF_INT32(svc_num_arg), OBF_INT32(nargs_arg), ##__VA_ARGS__)

#endif //TALESTARS_V63_322_SECURE_SYSCALL_H
