#ifndef TALESTARS_V63_322_JPROVIDER_H
#define TALESTARS_V63_322_JPROVIDER_H

#include <unstdinttypes.h>
#include <jni.h>
#include <stdatomic.h>
#include <unstdinttypes.h>

typedef struct JProvider_t {
    u64t magic;
    JavaVM *vm;
    JNIEnv *env;
    _Atomic(int) state;
} JProvider_t;

extern JProvider_t *meta_jprovider;

#endif //TALESTARS_V63_322_JPROVIDER_H
