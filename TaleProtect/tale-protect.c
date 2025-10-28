/*
 * Created by johan (TerryTheTerrible) on 23.09.25.
 * hours wasted: countless.
 * knowledge gained: measureless.
 * female interactions: affirmative.
 * revenue: null pointer dereference - signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0x0000000000000011
 *
 */

#include <global_common/common.h>
#include <global_common/junkcode.h>
//! [metas]
#include "TaleProtect/metas/jprovider.h"
//! [demolisher]
#include <TaleProtect/demolisher/rainbow_terminator.h>
//! [mappings/compat]
#include <TaleProtect/mappings/compat/dexmappings.h>
//! [func_resolver]
#include <TaleProtect/func_resolver/func_resolver_3000.h>
//! [resolvers::compat]
#include <TaleProtect/resolvers/compat/metadata_resolver.h>
//! [tools::segment_resolver]
#include <TaleProtect/tools/segment_resolver/segment_resolver.h>
//! [not_an_elf]
#include <TaleProtect/not_an_elf/not_an_elf.h>
//! [process_monitor::native_library_directory_entries_protection]
#include "TaleProtect/process_monitor/native_library_directory_entries_protection/native_library_directory_entries_protection.h"
//! [process_monitor::injection_detection]
#include "TaleProtect/process_monitor/injection_detection/wrap_sysprop_protection/wrap_sysprop_protection.h"
#include "TaleProtect/process_monitor/injection_detection/ld_preload_protection/ld_preload_protection.h"
#include "TaleProtect/process_monitor/injection_detection/cmdline_protection/cmdline_protection.h"
#include "TaleProtect/process_monitor/injection_detection/proc_exe_protection/proc_exe_protection.h"
//! [process_monitor::maps_monitor::maps_entries_protection]
#include "TaleProtect/process_monitor/maps_monitor/maps_entries_protection/maps_entries_protection.h"
//! [process_monitor::sniffer::memory_sniffer_protection]
//#include "TaleProtect/process_monitor/sniffer/memory_sniffer_protection/memory_sniffer_protection.h"
//#include "TaleProtect/process_monitor/sniffer/memory_sniffer_protection/memory_sniffer_jitcache_protection.h"
//! [process_monitor::tool_specific_mitigations::frida]
#include "TaleProtect/process_monitor/tool_specific_mitigations/frida/frida_protocol_protection/frida_protocol_protection.h"
//! [process_monitor::net_monitor]
#include "TaleProtect/process_monitor/net_monitor/net_monitor_protection.h"
#include "TaleProtect/process_monitor/threads_monitor/threads_name_protection/threads_name_protection.h"
//! [process_monitor::trace_monitor]
#include "TaleProtect/process_monitor/trace_monitor/already_ptraced_dude/already_ptraced_dude.h"
#include "TaleProtect/process_monitor/trace_monitor/tracerpid_protection/tracerpid_protection.h"
#include "TaleProtect/indirect_registration/indirect_registration.h"
#include "TaleProtect/resolvers/jprovider/jprovider_resolver.h"
//! [integrity_monitor::app_integrity]
#include <TaleProtect/integrity_monitor/app_integrity/app_integrity.h>
//! [integrity_monitor::code_integrity]
#include <TaleProtect/integrity_monitor/code_integrity/self/code_integrity_bootstrap_0.h>
#include <TaleProtect/integrity_monitor/code_integrity/self/code_integrity_trusted_0.h>
//! [integrity_monitor::code_integrity::external::gadgets]
#include <TaleProtect/integrity_monitor/code_integrity/external/gadgets/tale/code_integrity_gadget_tale_trusted.h>
#include <TaleProtect/integrity_monitor/code_integrity/external/gadgets/royale/code_integrity_gadget_royale_trusted_conditional.h>
//! [integrity_monitor::file_integrity::native_library_directory]
#include <TaleProtect/integrity_monitor/file_integrity/native_library_directory/native_library_directory_file_integrity.h>


ATTR_CTOR void context_init_functions(void) {
    funcresolver_3000_init_libc_thunks();
    TaleProtectContextInit();
    injdetection_wrap_sysprop_protection();
    already_ptraced_dude_getout_protection();
}

ATTR_CTOR void process_monitor_phase_0(void) {
    injdetection_ld_preload_protection();
    injdetection_cmdline_protection();
    injdetection_proc_exe_protection();
    maps_monitor_maps_entries_protection(); /* parted */
    native_library_directory_entries_protection(); /* parted */
}


ATTR_CTOR void resolver_jprovider(void) {
    jprovider_resolver();
    app_integrity_protection(); /* todo: make this dependent later on... */
}

ATTR_CTOR void phase_0_code_integrity_check_bootstrap(void) {
    code_integrity_bootstrap_0();
    // todo: uncomment these before release
    net_monitor_protection();
    threads_name_protection();
    tracerpid_protection();
    native_library_directory_entries_integrity_protection();
}

ATTR_CTOR void phase_1_config_decrypt(void) {
    not_an_elf_decrypt_config();
    code_integrity_gadget_tale_trusted();
    code_integrity_gadget_royale_trusted_conditional();
}

ATTR_CTOR void phase_1_self_integrity_checks(void) {
    //! todo: uncomment and update hash before release
    code_integrity_trusted_0(
            &TaleProtectContext.integrity_monitor.code_integrity.self.PT_LOADS[0]
    );

    //! todo: dont uncomment these, they are very heavy. should also be linked separately without any obfuscation.
    /*
    memory_sniffer_protection();
    memory_sniffer_jitcache_protection();
    frida_protocol_protection();
     */
}

/*
ATTR_CTOR void phase_2_external_integrity_checks() {
    code_integrity_gadget_tale_bootstrap();
}
*/

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved_arg) {
//    registration_thread_func(vm);
//    return JNI_VERSION_1_6;

    s32t jerr = OBF_INT32(JNI_ERR);
    u32t signal_val_ptx_acq = OBF_INT32(PUTEX_ACQUIRED);
    jcode08();
    jcode07();

    u32t signal_val_0 = OBF_INT32(98);
    putex_result_t pr = putex_expected_wait_timeout(
            &TaleProtectContext.process_monitor.maps_monitor.maps_entries_protection.putex,
            (u32t) DEOBF_INT32(signal_val_0),
            7, 0
    );

    junk001();

    if (pr == (putex_result_t) DEOBF_INT32(signal_val_ptx_acq)) {
        jcode09();
        LOGD("(MainThread) main thread has been satisfied.");
    } else {
        LOGE("(MainThread) main thread failed to satisfy: %d", pr);
        demolisher_rainbow_terminator();
        demolisher_stackoverflower();
        demolisher_smash();
        return (s32t) DEOBF_INT32(jerr);
    }

    u32t jv = OBF_INT32(JNI_VERSION_1_6);

    if (dexmappings_save_ref(vm) != 67) {
        return (s32t) DEOBF_INT32(jerr);
    }

    u32t signal_val = OBF_INT32(0x40);
    putex_expected_signal(
            &indirect_registration_ctx.registration_thread_func_ptr_ptx,
            (u32t) DEOBF_INT32(signal_val)
    );

    LOGD("awaiting release signal..");
    shemaphore_result_t shm_release_res = shemaphore_expected_wait_timeout(
            &indirect_registration_ctx.release_jnionload,
            67,
            4, 6
    );

    u32t signal_val_shm_acq = OBF_INT32(SHEMAPHORE_ACQUIRED);
    if (shm_release_res != (shemaphore_result_t) DEOBF_INT32(signal_val_shm_acq)) {
        LOGE("(MainThread) main thread failed to release: %d", shm_release_res);
        jcode10();
        return (s32t) DEOBF_INT32(jerr);
    } else {
        LOGD("(MainThread) main thread release successfully: %d", shm_release_res);
        return DEOBF_INT32(jv);
    }

    junk001();

    return DEOBF_INT32(jv);
}
