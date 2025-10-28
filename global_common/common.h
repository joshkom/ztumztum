#ifndef TALESTARS_V63_322_COMMON_H
#define TALESTARS_V63_322_COMMON_H

#include <unstdinttypes.h>
#include <android/log.h>
#include <stdio.h>


#define ATTR_USED __attribute__((used))
#define ATTR_CTOR __attribute__((constructor))
#define ATTR_ALWAYS_INLINE __attribute__((always_inline))
#define FORCE_INLINE static inline __attribute__((always_inline))
#define ATTR_VISIBLE __attribute__((visibility("default")))
//#define EXTERN_C_EXPORT ATTR_VISIBLE EXTERN_C

#ifdef __cplusplus
#  define EXTERN_C extern "C"
#  define EXTERN_C_BEGIN extern "C" {
#  define EXTERN_C_END }
#else
#  define EXTERN_C
#  define EXTERN_C_BEGIN
#  define EXTERN_C_END
#endif

#define type_mallocM(type) ((type *)malloc(sizeof(type)))
#define type_callocM(type) ((type *)calloc(1, sizeof(type)))

#define LOG_TAG "TaleProtect"

enum LOGGER_LogTypes {
    LOGGER_DEBUG = 3,
    LOGGER_ERROR = 6,
    LOGGER_INFO = 4,
    LOGGER_WARN = 5
};


//#define LOGD(...) ((void)__android_log_print(LOGGER_DEBUG, LOG_TAG, __VA_ARGS__))
//#define LOGE(...) ((void)__android_log_print(LOGGER_ERROR, LOG_TAG, __VA_ARGS__))
//#define LOGI(...) ((void)__android_log_print(LOGGER_INFO,  LOG_TAG, __VA_ARGS__))
//#define LOGW(...) ((void)__android_log_print(LOGGER_WARN,  LOG_TAG, __VA_ARGS__))
#define LOGD(...)
#define LOGE(...)
#define LOGI(...)
#define LOGW(...)

FORCE_INLINE void hexdump(const u8t *buffer_arg, sizet size_arg) {
//    i8 buf[3 * size_arg + 1];
//    i8 *p = buf;
//    for (sizet i = 0; i < size_arg; ++i) {
//        i32 wrote = sprintf(p, "%02x ", buffer_arg[i]);
//        p += wrote;
//    }
//    *p = '\0';
//    LOGI(" >    %s", buf);
}


#endif //TALESTARS_V63_322_COMMON_H
