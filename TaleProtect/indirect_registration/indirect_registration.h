//
// Created by johan on 11.10.25.
//

#ifndef TALESTARS_V63_322_INDIRECT_REGISTRATION_H
#define TALESTARS_V63_322_INDIRECT_REGISTRATION_H

#include "TaleProtect/tools/shemaphore/shemaphore.h"
#include "TaleProtect/tools/putex/putex.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"
#include "TaleProtect/demolisher/stackoverflower.h"
#include "TaleProtect/demolisher/smash.h"

typedef void *(*registration_thread_func_t)(void *arg);

typedef struct indirect_registration_ctx_s {
    registration_thread_func_t registration_thread_func_ptr;
    putex_t registration_thread_func_ptr_ptx;
//    putex_t jni_onload_release_ptx;


    shemaphore_expected_t release_jnionload;
} indirect_registration_ctx_t;

extern indirect_registration_ctx_t indirect_registration_ctx;

void indirect_registration_ctx_init(
        void
);

void indirect_registration_registration_thread_func_ptr_setter_invoker(
        void
);


void *indirect_registration_registerer_invoker(
        void *arg
);

#endif //TALESTARS_V63_322_INDIRECT_REGISTRATION_H
