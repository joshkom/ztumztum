#include "func_resolver_3000.h"
#include <TaleProtect/demolisher/page_molester.h>
#include <TaleProtect/tools/utils.h>
#include <TaleProtect/demolisher/smash.h>
#include <TaleProtect/demolisher/rainbow_terminator.h>
#include <TaleProtect/demolisher/stackoverflower.h>
#include <TaleProtect/hook_detector/hook_detector.h>
#include <TaleProtect/tools/utils.h>

#include <unstdinttypes.h>
#include <global_common/common.h>
#include <unstdmemory_compat.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <unstdbool.h>
#include <malloc.h>

#define funcresolver_3000_populate_funcM(enc_name_ptr_arg) {enc_name_ptr_arg, sizeof(enc_name_ptr_arg)/sizeof(enc_name_ptr_arg[0]), NULL, {0}}
#define HEAL_INTERVAL_MS (968)
//#define HEAL_INTERVAL_MS (168)
#define FR3_XOR_KEY 0xf1

// ============= [libc function wrappers (auto-generated via `thunk_gen.py`)] =============
static i8 enc_dlsym[] = {
        'd' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY, 's' ^ FR3_XOR_KEY, 'y' ^ FR3_XOR_KEY,
        'm' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_malloc[] = {
        'm' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY,
        'o' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_calloc[] = {
        'c' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY,
        'o' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_realloc[] = {
        'r' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY,
        'l' ^ FR3_XOR_KEY, 'o' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_free[] = {'f' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY,
                        0x0};

static i8 enc_fork[] = {'f' ^ FR3_XOR_KEY, 'o' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'k' ^ FR3_XOR_KEY,
                        0x0};

static i8 enc_mprotect[] = {
        'm' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'o' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_mmap[] = {'m' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY,
                        0x0};

static i8 enc_munmap[] = {
        'm' ^ FR3_XOR_KEY, 'u' ^ FR3_XOR_KEY, 'n' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY,
        'a' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_dladdr[] = {
        'd' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY,
        'd' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_dl_iterate_phdr[] = {
        'd' ^ FR3_XOR_KEY, 'l' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY,
        'h' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_read[] = {'r' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY,
                        0x0};

static i8 enc_getpagesize[] = {
        'g' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY,
        'a' ^ FR3_XOR_KEY, 'g' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 's' ^ FR3_XOR_KEY,
        'i' ^ FR3_XOR_KEY, 'z' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sysconf[] = {
        's' ^ FR3_XOR_KEY, 'y' ^ FR3_XOR_KEY, 's' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY,
        'o' ^ FR3_XOR_KEY, 'n' ^ FR3_XOR_KEY, 'f' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_pthread_create[] = {
        'p' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'h' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY,
        'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'c' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_pthread_detach[] = {
        'p' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'h' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY,
        'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'd' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY,
        'c' ^ FR3_XOR_KEY, 'h' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_pthread_once[] = {
        'p' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'h' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY,
        'e' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'd' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'o' ^ FR3_XOR_KEY, 'n' ^ FR3_XOR_KEY, 'c' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 0x0
};

static i8 enc___system_property_get[] = {
        '_' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY, 's' ^ FR3_XOR_KEY, 'y' ^ FR3_XOR_KEY,
        's' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY,
        '_' ^ FR3_XOR_KEY, 'p' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'o' ^ FR3_XOR_KEY,
        'p' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY,
        'y' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY, 'g' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_getenv[] = {
        'g' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY,
        'n' ^ FR3_XOR_KEY, 'v' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sem_init[] = {
        's' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'i' ^ FR3_XOR_KEY, 'n' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sem_post[] = {
        's' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'p' ^ FR3_XOR_KEY, 'o' ^ FR3_XOR_KEY, 's' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sem_wait[] = {
        's' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        'w' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sem_timedwait[] = {
        's' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY,
        'd' ^ FR3_XOR_KEY, 'w' ^ FR3_XOR_KEY, 'a' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 0x0
};

static i8 enc_sem_trywait[] = {
        's' ^ FR3_XOR_KEY, 'e' ^ FR3_XOR_KEY, 'm' ^ FR3_XOR_KEY, '_' ^ FR3_XOR_KEY,
        't' ^ FR3_XOR_KEY, 'r' ^ FR3_XOR_KEY, 'y' ^ FR3_XOR_KEY, 'w' ^ FR3_XOR_KEY,
        'a' ^ FR3_XOR_KEY, 'i' ^ FR3_XOR_KEY, 't' ^ FR3_XOR_KEY, 0x0
};

// ============= [libc function wrappers (auto-generated via `thunk_gen.py`)] =============
libc_func_entry_t libc_funcs[] = {
        funcresolver_3000_populate_funcM(enc_dlsym),
        funcresolver_3000_populate_funcM(enc_malloc),
        funcresolver_3000_populate_funcM(enc_calloc),
        funcresolver_3000_populate_funcM(enc_realloc),
        funcresolver_3000_populate_funcM(enc_free),
        funcresolver_3000_populate_funcM(enc_fork),
        funcresolver_3000_populate_funcM(enc_mprotect),
        funcresolver_3000_populate_funcM(enc_mmap),
        funcresolver_3000_populate_funcM(enc_munmap),
        funcresolver_3000_populate_funcM(enc_dladdr),
        funcresolver_3000_populate_funcM(enc_dl_iterate_phdr),
        funcresolver_3000_populate_funcM(enc_read),
        funcresolver_3000_populate_funcM(enc_getpagesize),
        funcresolver_3000_populate_funcM(enc_sysconf),
        funcresolver_3000_populate_funcM(enc_pthread_create),
        funcresolver_3000_populate_funcM(enc_pthread_detach),
        funcresolver_3000_populate_funcM(enc_pthread_once),
        funcresolver_3000_populate_funcM(enc___system_property_get),
        funcresolver_3000_populate_funcM(enc_getenv),
        funcresolver_3000_populate_funcM(enc_sem_init),
        funcresolver_3000_populate_funcM(enc_sem_post),
        funcresolver_3000_populate_funcM(enc_sem_wait),
        funcresolver_3000_populate_funcM(enc_sem_timedwait),
        funcresolver_3000_populate_funcM(enc_sem_trywait),
};

pthread_mutex_t libc_mprotect_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t libc_funcs_mutex[FR3_MAX_LIBC_FUNCS_COUNT] = {[0 ... FR3_MAX_LIBC_FUNCS_COUNT -
                                                                     1] = PTHREAD_MUTEX_INITIALIZER};


static volatile int funcresolver_3000_healer_running = 1;

i8 *funcresolver_3000_xor_decode(const i8 *buf, sizet len, u8t key) {
    if (!buf || len == 0) {
        return NULL;
    }

    i8 *result = (i8 *) malloc(len);
    if (!result) {
        return NULL;
    }

    for (sizet i = 0; i < len; i++) {
        result[i] = buf[i] ^ key;
    }

    result[len - 1] = 0;
    return result;
}

void funcresolver_3000_heal_function_prelude(void *func_ptr_arg,
                                             const void *stored_stub_arg,
                                             sizet page_size_arg,
                                             i8 *func_enc_name_arg,
                                             u8t func_enc_name_len_arg) {
    if (!func_ptr_arg) {
        return;
    }

    void *page = (void *) ((uptrt) func_ptr_arg & ~(page_size_arg - 1));
    mprotect(page, MAX_STUB_SIZE_BYTES, PROT_READ | PROT_WRITE | PROT_EXEC);

    tramp_info_t info;
    if (hook_detector_detect_trampoline_like(
            func_ptr_arg,
            8,
            8,
            &info)) {
        // todo: comment this before release
#ifndef ALL_LOGS_OFF
//        hook_detector_print_tramp_info(func_ptr_arg, &info);
#endif
        LOGE("trampoline-like pattern detected for %s", func_enc_name_arg);

//        demolisher_rainbow_terminator();

        if (unstdmemory_compat_memcmp(
                func_ptr_arg,
                stored_stub_arg,
                MAX_STUB_SIZE_BYTES) == 0) {
            LOGE("Well, here we are, left choice-less. lets go gambling");
            // todo: make a libg.so molester
            if (info.resolved_target) {
                demolisher_page_molester_set_page_readonly((void *) info.resolved_target);
            }

            demolisher_stackoverflower();
            demolisher_smash();
        }
    } else {
//        LOGI("no trampoline-like first-instruction patterns in %s", func_enc_name_arg);
    }

    if (unstdmemory_compat_memcmp(func_ptr_arg, stored_stub_arg, MAX_STUB_SIZE_BYTES) == 0) {
        return;
    }

    LOGE("hook detected at %p, dumping...", func_ptr_arg);

//    LOGE("hook on %s, dumping...", !*func_enc_name_arg ? "?unknown?" : func_enc_name_arg);
//    LOGI("Before hook:");
//    hexdump((u8t *) (stored_stub_arg), 4 * 4);
//    LOGI("After hook:");
//    hexdump((u8t *) (func_ptr_arg), 4 * 4);

    unstdmemory_compat_memcpy(func_ptr_arg, stored_stub_arg, MAX_STUB_SIZE_BYTES);
    mprotect(page, MAX_STUB_SIZE_BYTES, PROT_READ | PROT_EXEC);
}

void *funcresolver_3000_preludes_healer_thread(void *arg) {
    (void) arg;
    sizet page_size = sysconf(_SC_PAGESIZE);

    while (funcresolver_3000_healer_running) {
        for (sizet i = 0; i < sizeof(libc_funcs) / sizeof(libc_funcs[0]); i++) {
            funcresolver_3000_heal_function_prelude(
                    libc_funcs[i].resolved_ptr,
                    libc_funcs[i].prelude_ptr,
                    page_size,
                    (i8 *) libc_funcs[i].enc_name,
                    libc_funcs[i].enc_len
            );
        }
        utils_msleep(HEAL_INTERVAL_MS);
    }

    return NULL;
}

void funcresolver_3000_init_libc_thunks(void) {
    for (sizet i = 0; i < sizeof(libc_funcs) / sizeof(libc_funcs[0]); i++) {
        i8 *tmp_dec_sym_holder = funcresolver_3000_xor_decode(
                libc_funcs[i].enc_name,
                libc_funcs[i].enc_len,
                FR3_XOR_KEY
        );

        if (!i) {
            libc_funcs[i].resolved_ptr = dlsym(RTLD_DEFAULT, tmp_dec_sym_holder);
            LOGI("dlsym();");
        } else {
            libc_funcs[i].resolved_ptr = dlsymM(RTLD_DEFAULT, (char *) tmp_dec_sym_holder);
            LOGI("dlsymM();");
        }

        LOGE("resolved ptr for %s is %p", tmp_dec_sym_holder, libc_funcs[i].resolved_ptr);

        if (libc_funcs[i].resolved_ptr) {
            void *src = (void *) libc_funcs[i].resolved_ptr;
            void *dst = (void *) libc_funcs[i].prelude_ptr;
            size_t nbytes = (size_t) MAX_STUB_SIZE_BYTES;

            size_t page_size = (size_t) sysconf(_SC_PAGESIZE);
            if (page_size == (size_t) -1) page_size = 4096;

            uintptr_t src_start = (uintptr_t) src;
            uintptr_t src_end = src_start + (uintptr_t) nbytes - 1;

            void *mstart = utils_page_align_down_ptr((void *) src_start, page_size);
            size_t mlen = (size_t) ((src_end - (uintptr_t) mstart) + 1);

            if (mlen % page_size) {
                mlen = ((mlen / page_size) + 1) * page_size;
            }

            LOGI("attempting to mprotect %p..%p (len=%zu) to PROT_READ|PROT_EXEC",
                 mstart, (void *) ((uintptr_t) mstart + mlen), mlen);

            if (mprotect(mstart, mlen, PROT_READ | PROT_EXEC) != 0) {
                int err = errno;
                LOGE("mprotect(PROT_READ|PROT_EXEC) failed for %p (len=%zu): %s (%d). "
                     "Skipping memcpy to avoid SIGSEGV. Consider using a trampoline fallback.",
                     mstart, mlen, strerror(err), err);
            } else {
                /* now safe to copy */
                LOGI("source pages are readable -> performing memcpy from %p to %p (%zu bytes)",
                     src, dst, nbytes);

                unstdmemory_compat_memcpy(dst, src, nbytes);

                /* best-effort: restore to EXEC-only (remove READ) */
                if (mprotect(mstart, mlen, PROT_EXEC) != 0) {
                    int err2 = errno;
                    LOGW("mprotect(PROT_EXEC) restore failed for %p (len=%zu): %s (%d). "
                         "Pages may currently be READ|EXEC; continuing anyway.",
                         mstart, mlen, strerror(err2), err2);
                } else {
                    LOGI("restored pages to PROT_EXEC for %p..%p", mstart,
                         (void *) ((uintptr_t) mstart + mlen));
                }
            }


//            unstdmemory_compat_memcpy(
//                    libc_funcs[i].prelude_ptr,
//                    libc_funcs[i].resolved_ptr,
//                    MAX_STUB_SIZE_BYTES
//            );
        }

        unstdmemory_compat_memset(
                tmp_dec_sym_holder,
                0,
                libc_funcs[i].enc_len
        );
        free(tmp_dec_sym_holder);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, funcresolver_3000_preludes_healer_thread, NULL);
    pthread_detach(tid);
}

