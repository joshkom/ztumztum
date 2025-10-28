#ifndef TALESTARS_V63_322_PROC_EXE_PROTECTION_H
#define TALESTARS_V63_322_PROC_EXE_PROTECTION_H

#include <unstdinttypes.h>
#include <unstdpair.h>
#include <linux/limits.h>

typedef struct {
    int result;
    char exe_path[PATH_MAX + 1];
} injdetection_proc_exe_inspection_t;

injdetection_proc_exe_inspection_t injdetection_proc_exe_inspection(
        void
);

void injdetection_proc_exe_protection(
        void
);


#endif //TALESTARS_V63_322_PROC_EXE_PROTECTION_H
