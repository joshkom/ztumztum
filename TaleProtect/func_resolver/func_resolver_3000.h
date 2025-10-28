#ifndef TALESTARS_V63_322_FUNC_RESOLVER_3000_H
#define TALESTARS_V63_322_FUNC_RESOLVER_3000_H

#include <unstdinttypes.h>
#include <unstdmemory_compat.h>
#include <global_common/common.h>

#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <dlfcn.h>
#include <link.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>

#define MAX_STUB_SIZE_BYTES 16

typedef struct {
    i8 *_Nonnull enc_name;
    sizet enc_len;
    void *_Nullable resolved_ptr;
    i8 prelude_ptr[MAX_STUB_SIZE_BYTES];
    _Atomic int prelude_installed;
} libc_func_entry_t;

#define FR3_MAX_LIBC_FUNCS_COUNT 128
extern libc_func_entry_t libc_funcs[];
extern pthread_mutex_t libc_funcs_mutex[FR3_MAX_LIBC_FUNCS_COUNT];
extern pthread_mutex_t libc_mprotect_mutex;

EXTERN_C void funcresolver_3000_init_libc_thunks(
        void
);


#ifdef __cplusplus
}
#endif

// This code sucks, you know it and I know it.
// Now move on and call me an idiot later.
#define funcresolver_3000_wrapfunc_typedM(idx_arg, functype_arg)                                     \
({                                                                                                   \
    void *_ptr = (void *)libc_funcs[(idx_arg)].resolved_ptr;                                         \
                                                                                                     \
    if (atomic_load_explicit(&libc_funcs[(idx_arg)].prelude_installed,                               \
                             memory_order_acquire)) {                                                \
        (functype_arg)_ptr;                                                                          \
    }                                                                                                \
                                                                                                     \
    pthread_mutex_lock(&libc_funcs_mutex[(idx_arg)]);                                                \
    if (atomic_load_explicit(&libc_funcs[(idx_arg)].prelude_installed,                               \
                             memory_order_acquire)) {                                                \
        pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                          \
        (functype_arg)_ptr;                                                                          \
    }                                                                                                \
                                                                                                     \
    size_t _pagesize = (size_t)sysconf(_SC_PAGESIZE);                                                \
    void *_page_start = (void *)((uptrt)_ptr & ~(_pagesize - 1));                                    \
                                                                                                     \
    pthread_mutex_lock(&libc_mprotect_mutex);                                                        \
    if (mprotect(_page_start, _pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {                 \
        pthread_mutex_unlock(&libc_mprotect_mutex);                                                  \
        pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                          \
        (functype_arg)_ptr;                                                                          \
    }                                                                                                \
                                                                                                     \
    if (unstdmemory_compat_memcmp(_ptr, (const void *)libc_funcs[(idx_arg)].prelude_ptr, 16) != 0) { \
        unstdmemory_compat_memcpy(_ptr, (const void *)libc_funcs[(idx_arg)].prelude_ptr, 16);        \
    }                                                                                                \
                                                                                                     \
    (void)mprotect(_page_start, _pagesize, PROT_READ | PROT_EXEC);                                   \
    pthread_mutex_unlock(&libc_mprotect_mutex);                                                      \
                                                                                                     \
    atomic_store_explicit(&libc_funcs[(idx_arg)].prelude_installed, 1,                               \
                          memory_order_release);                                                     \
    pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                              \
                                                                                                     \
    (functype_arg)_ptr;                                                                              \
})


//! this code cries itself to death in xiaomi devices due to https://source.android.com/docs/security/test/execute-only-memory
//#define funcresolver_3000_wrapfunc_typedM(idx_arg, functype_arg)                                         \
//({                                                                                                       \
//    void *_ptr = (void *)libc_funcs[(idx_arg)].resolved_ptr;                                             \
//    if (unstdmemory_compat_memcmp(_ptr, (const void *)libc_funcs[(idx_arg)].prelude_ptr, 16) != 0) {     \
//        pthread_mutex_lock(&libc_funcs_mutex[(idx_arg)]);                                                \
//        if (unstdmemory_compat_memcmp(_ptr, (const void *)libc_funcs[(idx_arg)].prelude_ptr, 16) == 0) { \
//            pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                          \
//            (functype_arg)_ptr;                                                                          \
//        } else {                                                                                         \
//            size_t _pagesize = (size_t)sysconf(_SC_PAGESIZE);                                            \
//            void *_page_start = (void *)((uptrt)_ptr & ~(_pagesize - 1));                                \
//            pthread_mutex_lock(&libc_mprotect_mutex);                                                    \
//            if (mprotect(_page_start, _pagesize, PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {             \
//                pthread_mutex_unlock(&libc_mprotect_mutex);                                              \
//                pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                      \
//                (functype_arg)_ptr;                                                                      \
//            }                                                                                            \
//            unstdmemory_compat_memcpy(_ptr, (const void *)libc_funcs[(idx_arg)].prelude_ptr, 16);        \
//            (void)mprotect(_page_start, _pagesize, PROT_READ | PROT_EXEC);                               \
//            pthread_mutex_unlock(&libc_mprotect_mutex);                                                  \
//            pthread_mutex_unlock(&libc_funcs_mutex[(idx_arg)]);                                          \
//            (functype_arg)_ptr;                                                                          \
//        }                                                                                                \
//    } (functype_arg)_ptr;                                                                                \
//})

//! NON-THREAD-SAFE-ZONE !
// If this comment is removed the program will blow up
//#define funcresolver_3000_wrapfunc_inlineM(idx_arg, ret_type_arg, ...) \
//    (mprotect((void *)((uptrt)libc_funcs[idx_arg].resolved_ptr & ~(sysconf(_SC_PAGESIZE)-1)), 16, PROT_READ|PROT_WRITE|PROT_EXEC), \
//     unstdmemory_compat_memcpy(libc_funcs[idx_arg].resolved_ptr, libc_funcs[idx_arg].prelude_ptr, 16), \
//     mprotect((void *)((uptrt)libc_funcs[idx_arg].resolved_ptr & ~(sysconf(_SC_PAGESIZE)-1)), 16, PROT_READ|PROT_EXEC), \
//     ((ret_type_arg(*)(__VA_ARGS__))libc_funcs[idx_arg].resolved_ptr))
//#define funcresolver_3000_wrapfunc_typedM(idx_arg, functype_arg) \
//    (mprotect((void *)((uptrt)libc_funcs[idx_arg].resolved_ptr & ~(sysconf(_SC_PAGESIZE)-1)), 16, PROT_READ|PROT_WRITE|PROT_EXEC), \
//        unstdmemory_compat_memcpy(libc_funcs[idx_arg].resolved_ptr, libc_funcs[idx_arg].prelude_ptr, 16), \
//        mprotect((void *)((uptrt)libc_funcs[idx_arg].resolved_ptr & ~(sysconf(_SC_PAGESIZE)-1)), 16, PROT_READ|PROT_EXEC), \
//        ((functype_arg)libc_funcs[idx_arg].resolved_ptr))
//#define funcresolver_3000_wrapfunc_typedM(idx_arg, functype_arg) \
//({ \
//    pthread_mutex_lock(&libc_funcs_mutex[idx_arg]); \
//    size_t pagesize = sysconf(_SC_PAGESIZE); \
//    void *page_start = (void *)((uptrt)libc_funcs[idx_arg].resolved_ptr & ~(pagesize-1)); \
//    mprotect(page_start, pagesize, PROT_READ | PROT_WRITE | PROT_EXEC); \
//    unstdmemory_compat_memcpy(libc_funcs[idx_arg].resolved_ptr, libc_funcs[idx_arg].prelude_ptr, 16); \
//    mprotect(page_start, pagesize, PROT_READ | PROT_EXEC); \
//    functype_arg result = (functype_arg)libc_funcs[idx_arg].resolved_ptr; \
//    pthread_mutex_unlock(&libc_funcs_mutex[idx_arg]); \
//    result; \
//})



// ============= [libc function wrappers (auto-generated via `thunk_gen.py`)] =============
typedef void *_Nullable (*fr3_dlsym_t)(
        void *__BIONIC_COMPLICATED_NULLNESS __handle,
        const char *_Nullable __symbol
);                                                                   /* 0 */
#define dlsymM funcresolver_3000_wrapfunc_typedM(0, fr3_dlsym_t)

typedef void *_Nullable (*fr3_malloc_t)(
        size_t __byte_count
);                                                                   /* 1 */
#define mallocM funcresolver_3000_wrapfunc_typedM(1, fr3_malloc_t)

typedef void *_Nullable (*fr3_calloc_t)(
        size_t __item_count,
        size_t __item_size
);                                                                   /* 2 */
#define callocM funcresolver_3000_wrapfunc_typedM(2, fr3_calloc_t)

typedef void *_Nullable (*fr3_realloc_t)(
        void *_Nullable __ptr,
        size_t __byte_count
);                                                                   /* 3 */
#define reallocM funcresolver_3000_wrapfunc_typedM(3, fr3_realloc_t)

typedef void (*fr3_free_t)(
        void *_Nullable __ptr
);                                                                   /* 4 */
#define freeM funcresolver_3000_wrapfunc_typedM(4, fr3_free_t)

typedef pid_t (*fr3_fork_t)(
        void
);                                                                   /* 5 */
#define forkM funcresolver_3000_wrapfunc_typedM(5, fr3_fork_t)

typedef int (*fr3_mprotect_t)(
        void *_Nonnull __addr,
        size_t __size,
        int __prot
);                                                                   /* 6 */
#define mprotectM funcresolver_3000_wrapfunc_typedM(6, fr3_mprotect_t)

typedef void *_Nullable (*fr3_mmap_t)(
        void *_Nullable __addr,
        size_t __size,
        int __prot,
        int __flags,
        int __fd,
        off_t __offset
);                                                                   /* 7 */
#define mmapM funcresolver_3000_wrapfunc_typedM(7, fr3_mmap_t)

typedef int   (*fr3_munmap_t)(
        void *_Nonnull,
        sizet
);                                                                   /* 8 */
#define munmapM funcresolver_3000_wrapfunc_typedM(8, fr3_munmap_t)

typedef int   (*fr3_dladdr_t)(
        const void *_Nonnull,
        Dl_info *_Nonnull
);                                                                   /* 9 */
#define dladdrM funcresolver_3000_wrapfunc_typedM(9, fr3_dladdr_t)

typedef int (*fr3_dl_iterate_phdr_t)(
        int (*_Nonnull callback)(struct dl_phdr_info *_Nonnull, size_t, void *_Nullable),
        void *_Nullable data
);                                                                   /* 10 */
#define dl_iterate_phdrM funcresolver_3000_wrapfunc_typedM(10, fr3_dl_iterate_phdr_t)

typedef ssize_t (*fr3_read_t)(
        int fd,
        void *_Nonnull const __pass_object_size0 buf,
        size_t count
);                                                                   /* 11 */
#define readM funcresolver_3000_wrapfunc_typedM(11, fr3_read_t)

typedef int (*fr3_getpagesize_t)(
        void
);                                                                   /* 12 */
#define getpagesizeM funcresolver_3000_wrapfunc_typedM(12, fr3_getpagesize_t)

typedef long (*fr3_sysconf_t)(
        int __name
);                                                                   /* 13 */
#define sysconfM funcresolver_3000_wrapfunc_typedM(13, fr3_sysconf_t)

typedef int (*fr3_pthread_create_t)(
        pthread_t *_Nonnull __pthread_ptr,
        pthread_attr_t const *_Nullable __attr,
        void *_Nonnull (*_Nonnull __start_routine)(void *_Nonnull),
        void *_Nullable
);                                                                   /* 14 */
#define pthread_createM funcresolver_3000_wrapfunc_typedM(14, fr3_pthread_create_t)

typedef int (*fr3_pthread_detach_t)(
        pthread_t __pthread
);                                                                   /* 15 */
#define pthread_detachM funcresolver_3000_wrapfunc_typedM(15, fr3_pthread_detach_t)

typedef int (*fr3_pthread_once_t)(
        pthread_once_t *_Nonnull __once,
        void (*_Nonnull __init_routine)(void)
);                                                                   /* 16 */
#define pthread_onceM funcresolver_3000_wrapfunc_typedM(16, fr3_pthread_once_t)

typedef int (*fr3___system_property_get_t)(
        const char *_Nonnull __name,
        char *_Nonnull __value
);                                                                   /* 17 */
#define __system_property_getM funcresolver_3000_wrapfunc_typedM(17, fr3___system_property_get_t)

typedef char *_Nullable (*fr3_getenv_t)(
        const char *_Nonnull __name
);                                                                   /* 18 */
#define getenvM funcresolver_3000_wrapfunc_typedM(18, fr3_getenv_t)

typedef int (*fr3_sem_init_t)(
        sem_t *_Nonnull __sem,
        int __shared,
        unsigned int __value
);                                                                   /* 19 */
#define sem_initM funcresolver_3000_wrapfunc_typedM(19, fr3_sem_init_t)

typedef int (*fr3_sem_post_t)(
        sem_t *_Nonnull __sem
);                                                                   /* 20 */
#define sem_postM funcresolver_3000_wrapfunc_typedM(20, fr3_sem_post_t)

typedef int (*fr3_sem_wait_t)(
        sem_t *_Nonnull __sem
);                                                                   /* 21 */
#define sem_waitM funcresolver_3000_wrapfunc_typedM(21, fr3_sem_wait_t)

typedef int (*fr3_sem_timedwait_t)(
        sem_t *_Nonnull __sem,
        const struct timespec *_Nonnull __ts
);                                                                   /* 22 */
#define sem_timedwaitM funcresolver_3000_wrapfunc_typedM(22, fr3_sem_timedwait_t)

typedef int (*fr3_sem_trywait_t)(
        sem_t *_Nonnull __sem
);                                                                   /* 23 */
#define sem_trywaitM funcresolver_3000_wrapfunc_typedM(23, fr3_sem_trywait_t)

#endif /* TALESTARS_V63_322_FUNC_RESOLVER_3000_H */
