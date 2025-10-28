#include <linux/fcntl.h>
#include "proc_exe_protection.h"
#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/utils.h"
#include "TaleProtect/demolisher/smash.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"


//! /proc/self/exe
static inline __attribute__((always_inline)) unsigned char* decode_proc_self_exe__14(void){
    size_t __hc_len = 15u;
    unsigned char* buf = (unsigned char*)malloc(__hc_len);
    if(!buf) return NULL;
    volatile unsigned long long __hc_tmpA,__hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL; __hc_tmpA = (((( (__hc_tmpA * 0x000000009D5E4B49ULL) >> 38 ) | ( (__hc_tmpA * 0x000000009D5E4B49ULL) << 26 )) & 0xFFFFFFFFFFFFFFFFULL) ^ 0x0000000036970A05ULL);
    __hc_tmpB = 0xCBD8DBDC9C0BD95CULL; __hc_tmpB = ((( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) << 36 ) | ( ((( __hc_tmpB >> 50 ) | ( __hc_tmpB << 14 )) & 0xFFFFFFFFFFFFFFFFULL) >> 28 )) & 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char)((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char)((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL; __hc_tmpA = (((( (__hc_tmpA + 0x0000000073A5E55CULL) << 26 ) | ( (__hc_tmpA + 0x0000000073A5E55CULL) >> 38 )) & 0xFFFFFFFFFFFFFFFFULL) * 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BA13DD4BFF51EULL; __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char)((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = (unsigned char)((__hc_tmpB >> 8) & 0xFFULL);
    buf[10] = (unsigned char)((__hc_tmpB >> 16) & 0xFFULL);
    buf[11] = (unsigned char)((__hc_tmpB >> 24) & 0xFFULL);
    buf[12] = (unsigned char)((__hc_tmpB >> 32) & 0xFFULL);
    buf[13] = (unsigned char)((__hc_tmpB >> 40) & 0xFFULL);
    buf[14] = 0x0;
    return buf;
}

injdetection_proc_exe_inspection_t injdetection_proc_exe_inspection(void) {
    injdetection_proc_exe_inspection_t r = {0};
    r.result = 0;
    r.exe_path[0] = 0;


    const i8 *decoded_proc_self_exe = (i8 *) decode_proc_self_exe__14();

    //! [check /proc/self/exe]
    ssize_t ln = secure_syscall_v2M(
            SYS_readlinkat,
            4,
            AT_FDCWD,
            decoded_proc_self_exe,
            r.exe_path,
            PATH_MAX
    );

    secure_memory_wipe(
            (void *) decoded_proc_self_exe,
            14,
            SECURE_MEMORY_WIPE_FREE
    );

    if (ln < 0) {
        LOGD("(ProcessMonitor::InjectionDetection::ExeProtection::Inspection) readlinkat /proc/self/exe failed: %s", strerror(errno));
        return r;
    }

    r.exe_path[(ln < PATH_MAX) ? ln : PATH_MAX] = 0;
    LOGD("(ProcessMonitor::InjectionDetection::ExeProtection::Inspection) /proc/self/exe -> %s", r.exe_path);

    if (utils_is_path_suspicious(r.exe_path)) {
        LOGE("(ProcessMonitor::InjectionDetection::ExeProtection::Inspection) exe path suspicious: %s", r.exe_path);
        r.result = 1;
    }

    return r;
}

void injdetection_proc_exe_mitigation(injdetection_proc_exe_inspection_t buffer_arg) {
    if (buffer_arg.result != 0) {
        LOGE("(ProcessMonitor::InjectionDetection::ExeProtection::Mitigation) suspicious exe link detected...");
        demolisher_stackoverflower();
        demolisher_smash();
    }
}

void injdetection_proc_exe_protection(void) {
    injdetection_proc_exe_mitigation(
            injdetection_proc_exe_inspection()
    );
}
