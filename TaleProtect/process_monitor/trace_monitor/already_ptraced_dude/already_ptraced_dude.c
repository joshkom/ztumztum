
#include <linux/prctl.h>
#include "already_ptraced_dude.h"

#include "TaleProtect/secure_syscall/secure_syscall.h"
#include "TaleProtect/tools/utils.h"

//pid_t already_ptraced_dude_protection_2(void) {
//    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
//        if (errno == EPERM) {
//            LOGW("(TraceProtection::AlreadyPtracedDude) ptrace(PTRACE_TRACEME) failed: EPERM (already traced)");
//            return 1;
//        } else {
//            LOGW("(TraceProtection::AlreadyPtracedDude) ptrace(PTRACE_TRACEME) failed: %s",
//                 strerror(errno));
//            return -1;
//        }
//    }
//
//    LOGD("(TraceProtection::AlreadyPtracedDude) ptrace(PTRACE_TRACEME) succeeded");
//    ptrace(PTRACE_CONT, 0, NULL, NULL);
//    return 0;
//}


void already_ptraced_dude_getout_protection(void) {
    if ((i32) secure_syscall_v2M(SYS_prctl, 2, PR_SET_DUMPABLE, 0) < 0) {
        LOGW("(TraceProtection::AlreadyPtracedDude) prctl(PR_SET_DUMPABLE, 0); failed: %s",
             strerror(errno));
    } else {
        LOGD("(TraceProtection::AlreadyPtracedDude) prctl(PR_SET_DUMPABLE, 0); succeeded");
    }

#ifdef PR_SET_PTRACER
    if ((i32) secure_syscall_v2M(SYS_prctl, 2, PR_SET_PTRACER, -1) < 0) {
        LOGW("(TraceProtection::AlreadyPtracedDude) prctl(PR_SET_PTRACER, -1); failed: %s",
             strerror(errno));
    } else {
        LOGD("(TraceProtection::AlreadyPtracedDude) prctl(PR_SET_PTRACER, -1); succeeded");
    }
#endif
}
