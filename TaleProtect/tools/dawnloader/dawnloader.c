#include "dawnloader.h"
#include "TaleProtect/func_resolver/func_resolver_3000.h"

#include <jni.h>
#include <unstdbool.h>

dawnloader_result_t dawnloader(JNIEnv *env, const char *url_arg) {
    dawnloader_result_t result = {false, NULL, 0};

    // --- Set StrictMode ThreadPolicy ---
    jclass strictModeClass = (*env)->FindClass(env, "android/os/StrictMode");
    if (!strictModeClass) return result;

    jclass builderClass = (*env)->FindClass(env, "android/os/StrictMode$ThreadPolicy$Builder");
    if (!builderClass) return result;

    jmethodID builderCtor = (*env)->GetMethodID(env, builderClass, "<init>", "()V");
    jobject builder = (*env)->NewObject(env, builderClass, builderCtor);
    if (!builder) return result;

    jmethodID permitAllMethod = (*env)->GetMethodID(env, builderClass, "permitAll",
                                                    "()Landroid/os/StrictMode$ThreadPolicy$Builder;");
    builder = (*env)->CallObjectMethod(env, builder, permitAllMethod);

    jmethodID buildMethod = (*env)->GetMethodID(env, builderClass, "build",
                                                "()Landroid/os/StrictMode$ThreadPolicy;");
    jobject policy = (*env)->CallObjectMethod(env, builder, buildMethod);
    if (!policy) return result;

    jmethodID setThreadPolicyMethod = (*env)->GetStaticMethodID(env, strictModeClass,
                                                                "setThreadPolicy",
                                                                "(Landroid/os/StrictMode$ThreadPolicy;)V");
    (*env)->CallStaticVoidMethod(env, strictModeClass, setThreadPolicyMethod, policy);

    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return result;
    }

    // --- Prepare URL ---
    jclass URLClass = (*env)->FindClass(env, "java/net/URL");
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    jmethodID URL_ctor = (*env)->GetMethodID(env, URLClass, "<init>", "(Ljava/lang/String;)V");
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    jstring j_url = (*env)->NewStringUTF(env, url_arg);
    jobject url_obj = (*env)->NewObject(env, URLClass, URL_ctor, j_url);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    jmethodID openConnection = (*env)->GetMethodID(env, URLClass, "openConnection",
                                                   "()Ljava/net/URLConnection;");
    jobject conn_obj = (*env)->CallObjectMethod(env, url_obj, openConnection);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    // --- InputStream ---
    jclass URLConnClass = (*env)->FindClass(env, "java/net/URLConnection");
    jmethodID getInputStream = (*env)->GetMethodID(env, URLConnClass, "getInputStream",
                                                   "()Ljava/io/InputStream;");
    jobject input_stream = (*env)->CallObjectMethod(env, conn_obj, getInputStream);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    jclass InputStreamClass = (*env)->FindClass(env, "java/io/InputStream");
    jmethodID read = (*env)->GetMethodID(env, InputStreamClass, "read", "([B)I");
    jmethodID close = (*env)->GetMethodID(env, InputStreamClass, "close", "()V");

    // --- ByteArrayOutputStream ---
    jclass BAOSClass = (*env)->FindClass(env, "java/io/ByteArrayOutputStream");
    jmethodID baos_ctor = (*env)->GetMethodID(env, BAOSClass, "<init>", "()V");
    jmethodID baos_write = (*env)->GetMethodID(env, BAOSClass, "write", "([BII)V");
    jmethodID baos_toByteArray = (*env)->GetMethodID(env, BAOSClass, "toByteArray", "()[B");
    jmethodID baos_close = (*env)->GetMethodID(env, BAOSClass, "close", "()V");

    jobject baos = (*env)->NewObject(env, BAOSClass, baos_ctor);
    jbyteArray buffer = (*env)->NewByteArray(env, 8192);

    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    // --- Read loop ---
    size_t total_downloaded = 0;
    while (1) {
        jint read_len = (*env)->CallIntMethod(env, input_stream, read, buffer);
        if ((*env)->ExceptionCheck(env)) {
            (*env)->ExceptionClear(env);
            return result;
        }

        if (read_len == -1) break;

        (*env)->CallVoidMethod(env, baos, baos_write, buffer, 0, read_len);
        if ((*env)->ExceptionCheck(env)) {
            (*env)->ExceptionClear(env);
            return result;
        }

        total_downloaded += read_len;
    }

    jbyteArray result_array = (jbyteArray) (*env)->CallObjectMethod(env, baos, baos_toByteArray);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        return result;
    }

    jsize len = (*env)->GetArrayLength(env, result_array);
    jbyte *elements = (*env)->GetByteArrayElements(env, result_array, NULL);

    // Allocate and copy to C buffer
    result.data = (uint8_t *) mallocM(len);
    if (!result.data) {
        (*env)->ReleaseByteArrayElements(env, result_array, elements, JNI_ABORT);
        return result;
    }
    unstdmemory_compat_memcpy(result.data, elements, len);
    result.size = len;
    result.success = true;

    (*env)->ReleaseByteArrayElements(env, result_array, elements, JNI_ABORT);
    (*env)->CallVoidMethod(env, input_stream, close);
    (*env)->CallVoidMethod(env, baos, baos_close);

    return result;
}

dawnloader_result_t dawnloader_auto(JNIEnv *maybe_env, JavaVM *vm, const char *url_arg) {
    JNIEnv *env = maybe_env;
    bool did_attach = false;
    dawnloader_result_t res = {false, NULL, 0};

    if (env == NULL && vm != NULL) {
        jint getEnvStat = (*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6);
        if (getEnvStat == JNI_EDETACHED) {
            if ((*vm)->AttachCurrentThread(vm, &env, NULL) != 0) {
                return res;
            }
            did_attach = true;
        } else if (getEnvStat != JNI_OK) {
            return res;
        }
    }

    if (env == NULL) {
        return res;
    }

    res = dawnloader(env, url_arg);

    if (did_attach) {
        (*vm)->DetachCurrentThread(vm);
    }

    return res;
}
