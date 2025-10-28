//
// Created by johan on 12.10.25.
//

#ifndef TALESTARS_V63_322_DAWNLOADER_H
#define TALESTARS_V63_322_DAWNLOADER_H

#include <unstdinttypes.h>
#include <unstdbool.h>
#include <jni.h>

typedef struct dawnload_result_s {
    bool success;
    uint8_t *data;
    size_t size;
} dawnloader_result_t;

dawnloader_result_t dawnloader(
        JNIEnv *env,
        const char *url_arg
);

dawnloader_result_t dawnloader_auto(
        JNIEnv *maybe_env,
        JavaVM *vm,
        const char *url_arg
);

#endif //TALESTARS_V63_322_DAWNLOADER_H
