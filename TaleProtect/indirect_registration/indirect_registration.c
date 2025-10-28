//
// Created by johan on 11.10.25.
//

#include "indirect_registration.h"
#include "global_common/common.h"
#include "TaleProtect/mappings/compat/dexmappings.h"

indirect_registration_ctx_t indirect_registration_ctx = {0};


void indirect_registration_ctx_init(void) {
//    putex_init(&indirect_registration_ctx.jni_onload_release_ptx, 0);
    putex_init(&indirect_registration_ctx.registration_thread_func_ptr_ptx, 0);
    shemaphore_expected_init(&indirect_registration_ctx.release_jnionload, 0);
}

void *indirect_registration_registration_thread_func_ptr_setter(void *arg) {
    indirect_registration_ctx.registration_thread_func_ptr = (registration_thread_func_t) arg;
    LOGD("(IndirectRegistration::Setter) register callback is set;");
    return NULL;
}

void indirect_registration_registration_thread_func_ptr_setter_invoker(void) {
    pthread_t pthread;
    if (pthread_createM(&pthread, NULL,
                        indirect_registration_registration_thread_func_ptr_setter,
                        (void *) registration_thread_func
    )) {
        demolisher_stackoverflower();
        return;
    }
}


void *indirect_registration_registerer(void *arg) {
    (void) arg;
    u32t expected = 0x40;

    putex_result_t ptx_res = putex_expected_wait_timeout(
            &indirect_registration_ctx.registration_thread_func_ptr_ptx,
            expected,
            10,
            0
    );

    if (ptx_res != PUTEX_ACQUIRED) {
        LOGE("(IndirectRegistration) failed to received putex signal.");
        return NULL;
    }

    LOGD("(IndirectRegistration::Registerer) putex signal received successfully.");

    pthread_t pthread;
    if (pthread_createM(
            &pthread,
            NULL,
            indirect_registration_ctx.registration_thread_func_ptr,
            &expected)) {
        demolisher_stackoverflower();
    }

    if (pthread_detachM(pthread)) {
        demolisher_smash();
    }
}


void *indirect_registration_registerer_invoker(void *arg) {
    (void) arg;

    pthread_t pthread;
    if (pthread_createM(
            &pthread,
            NULL,
            indirect_registration_registerer,
            NULL)) {
        demolisher_stackoverflower();
    }

    if (pthread_detachM(pthread)) {
        demolisher_smash();
    }
}
