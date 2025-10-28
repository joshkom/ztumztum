#include <jni.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include "dexmappings.h"
#include "global_common/common.h"
#include "TaleProtect/metas/jprovider.h"
#include "TaleProtect/tools/shemaphore/shemaphore.h"
#include "TaleProtect/indirect_registration/indirect_registration.h"
#include "global_common/junkcode.h"
#include "TaleProtect/secure_memory_wipe/secure_memory_wipe.h"
#include <unstdbool.h>

jclass nzijsy_aq_class = NULL;
jclass nzijsy_ay_class = NULL;
jclass nzijsy_aC_class = NULL;
jclass nzijsy_J_class = NULL;
jclass nzijsy_az_class = NULL;

#include <android/log.h>

#define LOG_TAG_MAPPER "JNIHelper"
//#define DM_LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG_MAPPER, __VA_ARGS__)
//#define DM_ERR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG_MAPPER, __VA_ARGS__)
//#define DM_LOGI(...) LOGI(__VA_ARGS__)
//#define DM_ERR(...) LOGE(__VA_ARGS__)
#define DM_LOGI(...)
#define DM_ERR(...)

//#define LOG_TAG "NativeRegister"
//#define LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define DM_ERR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
//#define DM_LOG(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

/**
 * Try to get JNIEnv for current thread. Returns true if we need to detach later.
 */
static int try_get_env(JavaVM *vm_arg, JNIEnv **out_env_arg) {
    if ((*vm_arg)->GetEnv(vm_arg, (void **) out_env_arg, JNI_VERSION_1_6) == JNI_OK) {
        return 0; // already attached, no detach needed
    }

    if ((*vm_arg)->AttachCurrentThread(vm_arg, out_env_arg, NULL) != JNI_OK) {
        *out_env_arg = NULL;
        return 0;
    }

    return 1; // attached, need detach later
}

/**
 * Register native methods for a class
 */
static int register_methods(
        JNIEnv *env,
        const char *class_name_arg,
        JNINativeMethod *methods_arg,
        int methods_count_arg,
        jclass *out_clazz_arg
) {
    jclass global_clazz = *out_clazz_arg;

    if (!global_clazz) {
        jclass local_clazz = (*env)->FindClass(env, class_name_arg);
        if (!local_clazz) {
            DM_ERR("Register: class %s not found", class_name_arg);
            return 0;
        }

        global_clazz = (jclass) (*env)->NewGlobalRef(env, local_clazz);
        (*env)->DeleteLocalRef(env, local_clazz);

        if (!global_clazz) {
            DM_ERR("Register: failed to create global ref for %s", class_name_arg);
            return 0;
        }

        *out_clazz_arg = global_clazz;
    }

    if ((*env)->RegisterNatives(env, global_clazz, methods_arg, methods_count_arg) < 0) {
        DM_ERR("Register: failed for %s", class_name_arg);
        return 0;
    }

    DM_LOGI("Registered native methods for %s", class_name_arg);
    return 1;
}

/**
 * Macro to simplify class registration
 */
#define REGISTER_CLASS(env, class_name_arg, class_methods_arg, out_class_ref_arg) \
    register_methods(env, class_name_arg, class_methods_arg, \
        sizeof(class_methods_arg) / sizeof(class_methods_arg[0]), out_class_ref_arg)

/**
 * Thread function for registration
 */
void *registration_thread_func(void *arg) {
//    JavaVM *vm = (JavaVM *) arg;
    JavaVM *vm = meta_jprovider->vm;
    JNIEnv *local_env = NULL;

    int needs_detach = try_get_env(vm, &local_env);
    if (!local_env) {
        DM_ERR("Registration thread: failed to get or attach JNIEnv");
        return NULL;
    }

    int ok = 1;

    LOGI("registered 0");
    ok &= REGISTER_CLASS(local_env, "gobvpmes/aa", nzijsy_aq_methods, &nzijsy_aq_class);

    LOGI("registered 1");
    ok &= REGISTER_CLASS(local_env, "gobvpmes/x", nzijsy_ay__spyware_0_methods, &nzijsy_ay_class);

    LOGI("registered 2");
    ok &= REGISTER_CLASS(local_env, "gobvpmes/aJ",
                         nzijsy_aC__getAppAttestationResponseToken_methods,
                         &nzijsy_aC_class);

    LOGI("registered 3");
    ok &= REGISTER_CLASS(local_env, "gobvpmes/d", nzijsy_J__bollocks_methods, &nzijsy_J_class);

    LOGI("registered 4");
    ok &= REGISTER_CLASS(local_env, "gobvpmes/aw", nzijsy_az__spyware_1_methods, &nzijsy_az_class);

    LOGI("registered 5");

    if (!ok) {
        DM_ERR("One or more classes failed to register native methods");
        if (needs_detach) {
            (*vm)->DetachCurrentThread(vm);
        }
        return NULL;
    }

    DM_LOGI("+++ALL CLASSES REGISTERED SUCCESSFULLY+++");

    if (needs_detach) {
        (*vm)->DetachCurrentThread(vm);
    }

    shemaphore_expected_signal(
            &indirect_registration_ctx.release_jnionload, 67
    );

    LOGD("(Register) release signal sent.");
    return NULL;
}

#include <jni.h>
#include <stdbool.h>


/**
 * Set a static object field
 */
void
setStaticObject(JNIEnv *env, jclass clazz, const char *name, jobject obj, const char *typeSig) {
    if (!clazz) {
        DM_ERR("setStaticObject: clazz is NULL for field '%s'", name);
        return;
    }
    jfieldID fid = (*env)->GetStaticFieldID(env, clazz, name, typeSig);
    if (!fid) {
        DM_ERR("setStaticObject: field '%s' with type '%s' not found", name, typeSig);
        return;
    }
    (*env)->SetStaticObjectField(env, clazz, fid, obj);
    DM_LOGI("setStaticObject: field '%s' set", name);
}

/**
 * Create java.util.ArrayList from a C string array
 */
jobject createArrayList(JNIEnv *env, const char **items, size_t count) {
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    if (!arrayListClass) {
        DM_ERR("createArrayList: ArrayList class not found");
        return NULL;
    }

    jmethodID ctor = (*env)->GetMethodID(env, arrayListClass, "<init>", "()V");
    if (!ctor) {
        DM_ERR("createArrayList: ArrayList constructor not found");
        return NULL;
    }

    jobject list = (*env)->NewObject(env, arrayListClass, ctor);
    if (!list) {
        DM_ERR("createArrayList: failed to create ArrayList object");
        return NULL;
    }

    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add", "(Ljava/lang/Object;)Z");
    if (!addMethod) {
        DM_ERR("createArrayList: add method not found");
        (*env)->DeleteLocalRef(env, list);
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        jstring jstr = (*env)->NewStringUTF(env, items[i]);
        if (!jstr) {
            DM_ERR("createArrayList: failed to create jstring for '%s'", items[i]);
            continue;
        }
        (*env)->CallBooleanMethod(env, list, addMethod, jstr);
        (*env)->DeleteLocalRef(env, jstr);
    }

    DM_LOGI("createArrayList: created list with %zu items", count);
    return list;
}

/**
 * Get a static object field from a class by name
 */
jobject getCompanionObject(JNIEnv *env, const char *className, const char *fieldName,
                           const char *fieldSig) {
    jclass targetClass = (*env)->FindClass(env, className);
    if (!targetClass) {
        DM_ERR("getCompanionObject: class '%s' not found", className);
        return NULL;
    }

    jfieldID fid = (*env)->GetStaticFieldID(env, targetClass, fieldName, fieldSig);
    if (!fid) {
        DM_ERR("getCompanionObject: field '%s' with type '%s' not found", fieldName, fieldSig);
        (*env)->DeleteLocalRef(env, targetClass);
        return NULL;
    }

    jobject obj = (*env)->GetStaticObjectField(env, targetClass, fid);
    DM_LOGI("getCompanionObject: got object for '%s.%s'", className, fieldName);

    (*env)->DeleteLocalRef(env, targetClass);
    return obj;
}

/**
 * Set static int field
 */
void setStaticInt(JNIEnv *env, jclass clazz, const char *name, int value) {
    if (!clazz) {
        DM_ERR("setStaticInt: clazz is NULL for field '%s'", name);
        return;
    }

    jfieldID fid = (*env)->GetStaticFieldID(env, clazz, name, "I");
    if (!fid) {
        DM_ERR("setStaticInt: field '%s' not found in class", name);
        return;
    }

    (*env)->SetStaticIntField(env, clazz, fid, value);
    DM_LOGI("setStaticInt: field '%s' set to %d", name, value);
}

/**
 * Set static string field
 */
void setStaticString(JNIEnv *env, jclass clazz, const char *name, const char *value) {
    if (!clazz) {
        DM_ERR("setStaticString: clazz is NULL for field '%s'", name);
        return;
    }

    jfieldID fid = (*env)->GetStaticFieldID(env, clazz, name, "Ljava/lang/String;");
    if (!fid) {
        DM_ERR("setStaticString: field '%s' not found", name);
        return;
    }

    jstring str = value ? (*env)->NewStringUTF(env, value) : NULL;
    (*env)->SetStaticObjectField(env, clazz, fid, str);
    if (str) (*env)->DeleteLocalRef(env, str);

    DM_LOGI("setStaticString: field '%s' set to '%s'", name, value ? value : "NULL");
}

/**
 * Set static boolean field
 */
void setStaticBoolean(JNIEnv *env, jclass clazz, const char *name, bool value) {
    if (!clazz) {
        DM_ERR("setStaticBoolean: clazz is NULL for field '%s'", name);
        return;
    }

    jfieldID fid = (*env)->GetStaticFieldID(env, clazz, name, "Z");
    if (!fid) {
        DM_ERR("setStaticBoolean: field '%s' not found", name);
        return;
    }

    (*env)->SetStaticBooleanField(env, clazz, fid, value ? JNI_TRUE : JNI_FALSE);
    DM_LOGI("setStaticBoolean: field '%s' set to %s", name, value ? "true" : "false");
}

/**
 * Create AtomicBoolean object
 */
jobject createAtomicBoolean(JNIEnv *env, bool value) {
    jclass abClass = (*env)->FindClass(env, "java/util/concurrent/atomic/AtomicBoolean");
    if (!abClass) {
        DM_ERR("createAtomicBoolean: AtomicBoolean class not found");
        return NULL;
    }

    jmethodID ctor = (*env)->GetMethodID(env, abClass, "<init>", "(Z)V");
    if (!ctor) {
        DM_ERR("createAtomicBoolean: constructor not found");
        (*env)->DeleteLocalRef(env, abClass);
        return NULL;
    }

    jobject obj = (*env)->NewObject(env, abClass, ctor, value ? JNI_TRUE : JNI_FALSE);
    DM_LOGI("createAtomicBoolean: created object with value %s", value ? "true" : "false");

    (*env)->DeleteLocalRef(env, abClass);
    return obj;
}

/**
 * Get string from stringTable safely
 */
jstring nzijsy_aq_a_get_string(JNIEnv *env, jobject reserved, jint index) {
    if (index < 0 || (size_t) index >= stringTable_count) {
        DM_ERR("nzijsy_aq_a_get_string: index %d out of bounds", index);
        return (*env)->NewStringUTF(env, "");
    }
    DM_LOGI("nzijsy_aq_a_get_string: index %d inside bounds", index);

    return (*env)->NewStringUTF(env, stringTable[index]);
}


void nzijsy_aq_a_initialize_class(JNIEnv *env, jobject reserved, jclass clazz, jint code) {
//    LOGI("clazz code: %d", code);
//    auto setStaticInt = [&](const char *name, int value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "I");
//        if (fid) (*env)->SetStaticIntField(clazz, fid, value);
//    };
//
//    auto setStaticString = [&](const char *name, const char *value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Ljava/lang/String;");
//        if (fid) {
//            jstring str = value ? (*env)->NewStringUTF(value) : NULL;
//            (*env)->SetStaticObjectField(clazz, fid, str);
//        }
//    };

//    auto setStaticString = [&](const char *name, const char *value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Ljava/lang/String;");
//        if (fid) {
//            jstring str = value ? (*env)->NewStringUTF(value) : NULL;
//            (*env)->SetStaticObjectField(clazz, fid, str);
//            if (str) (*env)->DeleteLocalRef(env, str); // cleanup
//        }
//    };

//    DM_LOG("skew_a_cls + code: %lu ||| MetaTaleProtect.skew_cls + code: %lu: ", skew_a_cls + code, MetaTaleProtect.skew_cls + code);
//    if (skew_a_cls + code != MetaTaleProtect.skew_cls + code) {
//        taletools::memory_manipulators::smash_at();
//    }

//    auto setStaticBoolean = [&](const char *name, bool value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Z");
//        if (fid) (*env)->SetStaticBooleanField(clazz, fid, value ? JNI_TRUE : JNI_FALSE);
//    };
//
//    auto setStaticObject = [&](const char *name, jobject obj, const char *typeSig) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, typeSig);
//        if (fid) (*env)->SetStaticObjectField(clazz, fid, obj);
//    };
//
//    auto createArrayList = [&](std::vector<const char *> items) -> jobject {
//        jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
//        jmethodID arrayListCtor = (*env)->GetMethodID(env, arrayListClass, "<init>", "()V");
//        jobject list = (*env)->NewObject(env, arrayListClass, arrayListCtor);
//
//        jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add", "(Ljava/lang/Object;)Z");
//        for (const char *item: items) {
//            jstring jstr = (*env)->NewStringUTF(item);
//            (*env)->CallBooleanMethod(list, addMethod, jstr);
//            (*env)->DeleteLocalRef(env, jstr);
//        }
//        return list;
//    };
//
//    auto createAtomicBoolean = [&](bool value) -> jobject {
//        jclass abClass = (*env)->FindClass(env, "java/util/concurrent/atomic/AtomicBoolean");
//        jmethodID ctor = (*env)->GetMethodID(env, abClass, "<init>", "(Z)V");
//        return (*env)->NewObject(env, abClass, ctor, value ? JNI_TRUE : JNI_FALSE);
//    };
//
//    auto getCompanionObject = [&](const char *className, const char *fieldName,
//                                  const char *fieldSig) -> jobject {
//        jclass targetClass = (*env)->FindClass(className);
//        if (!targetClass) return NULL;
//        jfieldID fid = (*env)->GetStaticFieldID(targetClass, fieldName, fieldSig);
//        if (!fid) return NULL;
//        return (*env)->GetStaticObjectField(targetClass, fid);
//    };

//    void setStaticInt(JNIEnv* env, jclass clazz, const char* name, jint value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "I");
//        (*env)->SetStaticIntField(clazz, fid, value);
//    }
//
//    void setStaticBool(JNIEnv* env, jclass clazz, const char* name, jboolean value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Z");
//        (*env)->SetStaticBooleanField(clazz, fid, value);
//    }
//
//    void setStaticString(JNIEnv* env, jclass clazz, const char* name, const char* value) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Ljava/lang/String;");
//        jstring jstr = (*env)->NewStringUTF(value);
//        (*env)->SetStaticObjectField(clazz, fid, jstr);
//    }
//
//    void setStaticNull(JNIEnv* env, jclass clazz, const char* name) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Ljava/lang/Object;");
//        (*env)->SetStaticObjectField(clazz, fid, NULL);
//    }
//
//    void setStaticObject(JNIEnv* env, jclass clazz, const char* name, jobject obj) {
//        jfieldID fid = (*env)->GetStaticFieldID(clazz, name, "Ljava/lang/Object;");
//        (*env)->SetStaticObjectField(clazz, fid, obj);
//    }


//    switch (code) {
//        case 0x11: { // NetworkInfo
//            struct { const char* name; int value; } fields[] = {
//                    {"TRANSPORT_UNKNOWN",  0},
//                    {"TRANSPORT_NONE",     1},
//                    {"TRANSPORT_OTHER",    2},
//                    {"TRANSPORT_CELLULAR", 3},
//                    {"TRANSPORT_WIFI",     4},
//                    {"TRANSPORT_VPN",      5}
//            };
//            for (auto &f : fields) setStaticInt(f.name, f.value);
//            break;
//        }
//
//        case 0x1b: { // UsercentricsInitializer
//            setStaticString(env, clazz,  "LOG_TAG", "UsercentricsInitializer");
//            break;
//        }
//
//        case 0x1a: { // UsercentricsSDK
//            setStaticString(env, clazz,  "LOG_TAG", "UsercentricsSDK");
//            break;
//        }
//
//        case 0x5: { // GameApp
//            struct { const char* name; int value; } fields[] = {
//                    {"LOG_OUT_REASON_UNKNOWN", 0},
//                    {"LOG_OUT_REASON_USER", 1},
//                    {"LOG_OUT_REASON_AUTH_EXPIRED", 2},
//                    {"LOG_OUT_REASON_SWITCH_USER", 3},
//                    {"LOG_OUT_REASON_LOGIN_TIMEOUT", 4},
//                    {"LOG_OUT_REASON_NOT_IN_WHITE_LIST", 5},
//                    {"LOG_OUT_REASON_ANTI_ADDICTION", 6},
//                    {"LOG_OUT_REASON_CANCEL_QQ_AUTH", 7},
//                    {"LOG_OUT_REASON_CANCEL_WECHAT_AUTH", 8},
//                    {"LOG_OUT_REASON_DENY_WECHAT_AUTH", 9},
//                    {"SHOW_STORE_PAGE_REQUEST_CODE", 990001},
//                    {"ScreenOrientation_Portrait", 1},
//                    {"ScreenOrientation_PortraitUpsideDown", 2},
//                    {"ScreenOrientation_LandscapeLeft", 4},
//                    {"ScreenOrientation_LandscapeRight", 8},
//            };
//            for (auto &f : fields) setStaticInt(f.name, f.value);
//            break;
//        }
//
//        case 0x4: { // ApplicationUtilBase
//            setStaticString(env, clazz,  "PREFERENCES_NEW", "storage_new");
//            setStaticString(env, clazz,  "PREFERENCES_OLD", "storage");
//            setStaticInt(env, clazz, "b", -267522035);
//            break;
//        }
//
//        case 0x17: { // ApplicationUtil
//            setStaticString(env, clazz,  "PREFERENCES_NEW", "storage_new");
//            setStaticString(env, clazz,  "PREFERENCES_OLD", "storage");
//            setStaticInt(env, clazz, "b", -267522035);
//            break;
//        }
//
//        case 0x3: { // HelpshiftTitan
//            setStaticInt(env, clazz, "ORIENTATION_UNSPECIFIED", 0);
//            setStaticInt(env, clazz, "ORIENTATION_PORTRAIT", 1);
//            setStaticInt(env, clazz, "ORIENTATION_LANDSCAPE", 2);
//            break;
//        }
//
//        case 0x2: { // PurchaseManagerGoogle
//            setStaticInt(env, clazz, "PURCHASE_REQUEST_CODE", 10000004);
//            break;
//        }
//
//        case 0xe: { // R$id
//            struct { const char* name; int value; } fields[] = {
//                    {"InfoBox", 2131230725},
//                    {"infoBoxButton", 2131230984},
//                    {"infoBoxText", 2131230985},
//                    {"stage", 2131231177}
//            };
//            for (auto &f : fields) setStaticInt(f.name, f.value);
//            break;
//        }
//
//        case 0xf: { // NativeDialogManager
//            setStaticString(env, clazz,  "VISIBLE_NATIVE_DIALOG_TAG", "NativeDialog");
//            setStaticInt(env, clazz, "c", 1000);
//            break;
//        }
//
    switch (code) {
        case 0: {
            // Class: com.supercell.titan.R$string
            // Initialize all static int fields using setStaticInt helper
            setStaticInt(env, clazz, "UnsatisfiedLinkErrorReinstallPrompt", 2131623936);
            setStaticInt(env, clazz, "UnsatisfiedLinkErrorReinstallPromptKunlun", 2131623937);
            setStaticInt(env, clazz, "cadpa_text", 2131623972);
            setStaticInt(env, clazz, "default_notification_channel_id", 2131624005);
            setStaticInt(env, clazz, "facebook_application_id", 2131624012);
            setStaticInt(env, clazz, "helpshift_apiKey", 2131624024);
            setStaticInt(env, clazz, "helpshift_appId", 2131624025);
            setStaticInt(env, clazz, "helpshift_domain", 2131624026);
            setStaticInt(env, clazz, "hs__agent_message_voice_over", 2131624028);
            setStaticInt(env, clazz, "hs__agent_message_with_name_voice_over", 2131624029);
            setStaticInt(env, clazz, "hs__app_review_button", 2131624030);
            setStaticInt(env, clazz, "hs__attach_screenshot_btn", 2131624031);
            setStaticInt(env, clazz, "hs__attachment_downloaded__voice_over", 2131624032);
            setStaticInt(env, clazz, "hs__attachment_downloading_voice_over", 2131624033);
            setStaticInt(env, clazz, "hs__attachment_not_downloaded_voice_over", 2131624034);
            setStaticInt(env, clazz, "hs__authentication_failed", 2131624035);
            setStaticInt(env, clazz, "hs__ca_msg", 2131624036);
            setStaticInt(env, clazz, "hs__change_btn", 2131624037);
            setStaticInt(env, clazz, "hs__chat_hint", 2131624038);
            setStaticInt(env, clazz, "hs__confirmation_footer_msg", 2131624039);
            setStaticInt(env, clazz, "hs__confirmation_msg", 2131624040);
            setStaticInt(env, clazz, "hs__connecting", 2131624041);
            setStaticInt(env, clazz, "hs__contact_us_btn", 2131624042);
            setStaticInt(env, clazz, "hs__conversation_detail_error", 2131624043);
            setStaticInt(env, clazz, "hs__conversation_end_msg", 2131624044);
            setStaticInt(env, clazz, "hs__conversation_header", 2131624045);
            setStaticInt(env, clazz, "hs__conversation_issue_id_header", 2131624046);
            setStaticInt(env, clazz, "hs__conversation_publish_id_voice_over", 2131624047);
            setStaticInt(env, clazz, "hs__conversation_redacted_status", 2131624048);
            setStaticInt(env, clazz, "hs__conversation_redacted_status_multiple", 2131624049);
            setStaticInt(env, clazz, "hs__conversation_rejected_status", 2131624050);
            setStaticInt(env, clazz, "hs__conversation_started_message", 2131624051);
            setStaticInt(env, clazz, "hs__conversations_divider_voice_over", 2131624052);
            setStaticInt(env, clazz, "hs__copied_to_clipboard", 2131624053);
            setStaticInt(env, clazz, "hs__copy", 2131624054);
            setStaticInt(env, clazz, "hs__copy_to_clipboard_tooltip", 2131624055);
            setStaticInt(env, clazz, "hs__could_not_open_attachment_msg", 2131624056);
            setStaticInt(env, clazz, "hs__could_not_reach_support_msg", 2131624057);
            setStaticInt(env, clazz, "hs__cr_msg", 2131624058);
            setStaticInt(env, clazz, "hs__csat_additonal_feedback_message", 2131624059);
            setStaticInt(env, clazz, "hs__csat_dislike_message", 2131624060);
            setStaticInt(env, clazz, "hs__csat_like_message", 2131624061);
            setStaticInt(env, clazz, "hs__csat_message", 2131624062);
            setStaticInt(env, clazz, "hs__csat_option_message", 2131624063);
            setStaticInt(env, clazz, "hs__csat_rating_value_one", 2131624064);
            setStaticInt(env, clazz, "hs__csat_rating_value_other", 2131624065);
            setStaticInt(env, clazz, "hs__csat_ratingbar", 2131624066);
            setStaticInt(env, clazz, "hs__csat_submit_toast", 2131624067);
            setStaticInt(env, clazz, "hs__data_not_found_msg", 2131624068);
            setStaticInt(env, clazz, "hs__date_input_validation_error", 2131624069);
            setStaticInt(env, clazz, "hs__default_notification_channel_name", 2131624070);
            setStaticInt(env, clazz, "hs__default_notification_content_title", 2131624071);
            setStaticInt(env, clazz, "hs__description_invalid_length_error", 2131624072);
            setStaticInt(env, clazz, "hs__done_btn", 2131624073);
            setStaticInt(env, clazz, "hs__email_hint", 2131624074);
            setStaticInt(env, clazz, "hs__email_input_validation_error", 2131624075);
            setStaticInt(env, clazz, "hs__email_required_hint", 2131624076);
            setStaticInt(env, clazz, "hs__faq_fetching_fail_message", 2131624077);
            setStaticInt(env, clazz, "hs__faqs_search_footer", 2131624078);
            setStaticInt(env, clazz, "hs__feedback_button", 2131624079);
            setStaticInt(env, clazz, "hs__fetching_faqs_message", 2131624080);
            setStaticInt(env, clazz, "hs__file_not_found_msg", 2131624081);
            setStaticInt(env, clazz, "hs__help_header", 2131624082);
            setStaticInt(env, clazz, "hs__image_downloaded_voice_over", 2131624083);
            setStaticInt(env, clazz, "hs__image_downloading_voice_over", 2131624084);
            setStaticInt(env, clazz, "hs__image_not_downloaded_voice_over", 2131624085);
            setStaticInt(env, clazz, "hs__invalid_description_error", 2131624086);
            setStaticInt(env, clazz, "hs__invalid_email_error", 2131624087);
            setStaticInt(env, clazz, "hs__invalid_faq_publish_id_error", 2131624088);
            setStaticInt(env, clazz, "hs__invalid_section_publish_id_error", 2131624089);
            setStaticInt(env, clazz, "hs__issue_archival_message", 2131624090);
            setStaticInt(env, clazz, "hs__jump_button_voice_over", 2131624091);
            setStaticInt(env, clazz, "hs__jump_button_with_new_message_voice_over", 2131624092);
            setStaticInt(env, clazz, "hs__landscape_date_input_validation_error", 2131624093);
            setStaticInt(env, clazz, "hs__landscape_email_input_validation_error", 2131624094);
            setStaticInt(env, clazz, "hs__landscape_input_validation_dialog_title", 2131624095);
            setStaticInt(env, clazz, "hs__landscape_number_input_validation_error", 2131624096);
            setStaticInt(env, clazz, "hs__mark_no", 2131624097);
            setStaticInt(env, clazz, "hs__mark_yes", 2131624098);
            setStaticInt(env, clazz, "hs__mark_yes_no_question", 2131624099);
            setStaticInt(env, clazz, "hs__message_not_sent", 2131624100);
            setStaticInt(env, clazz, "hs__messages_loading_error_text", 2131624101);
            setStaticInt(env, clazz, "hs__messages_loading_text", 2131624102);
            setStaticInt(env, clazz, "hs__network_error_msg", 2131624103);
            setStaticInt(env, clazz, "hs__network_error_pre_issue_creation", 2131624104);
            setStaticInt(env, clazz, "hs__network_reconnecting_error", 2131624105);
            setStaticInt(env, clazz, "hs__network_unavailable_msg", 2131624106);
            setStaticInt(env, clazz, "hs__new_conversation_btn", 2131624107);
            setStaticInt(env, clazz, "hs__new_conversation_footer_generic_reason", 2131624108);
            setStaticInt(env, clazz, "hs__new_conversation_header", 2131624109);
            setStaticInt(env, clazz, "hs__new_conversation_hint", 2131624110);
            setStaticInt(env, clazz, "hs__no_internet_error", 2131624111);
            setStaticInt(env, clazz, "hs__no_internet_error_mgs", 2131624112);
            setStaticInt(env, clazz, "hs__no_search_results_message", 2131624113);
            setStaticInt(env, clazz, "hs__notification_content_title_one", 2131624114);
            setStaticInt(env, clazz, "hs__notification_content_title_other", 2131624115);
            setStaticInt(env, clazz, "hs__number_input_validation_error", 2131624116);
            setStaticInt(env, clazz, "hs__permission_denied_message", 2131624117);
            setStaticInt(env, clazz, "hs__permission_denied_snackbar_action", 2131624118);
            setStaticInt(env, clazz, "hs__permission_not_granted", 2131624119);
            setStaticInt(env, clazz, "hs__permission_rationale_snackbar_action_label", 2131624120);
            setStaticInt(env, clazz, "hs__question_helpful_message", 2131624121);
            setStaticInt(env, clazz, "hs__question_unhelpful_message", 2131624122);
            setStaticInt(env, clazz, "hs__rate_button", 2131624123);
            setStaticInt(env, clazz, "hs__remove_screenshot_btn", 2131624124);
            setStaticInt(env, clazz, "hs__retry_button_voice_over", 2131624125);
            setStaticInt(env, clazz, "hs__retry_faq_fetching_button", 2131624126);
            setStaticInt(env, clazz, "hs__review_close_button", 2131624127);
            setStaticInt(env, clazz, "hs__review_message", 2131624128);
            setStaticInt(env, clazz, "hs__review_request_message", 2131624129);
            setStaticInt(env, clazz, "hs__review_title", 2131624130);
            setStaticInt(env, clazz, "hs__screenshot_add", 2131624131);
            setStaticInt(env, clazz, "hs__screenshot_cloud_attach_error", 2131624132);
            setStaticInt(env, clazz, "hs__screenshot_limit_error", 2131624133);
            setStaticInt(env, clazz, "hs__screenshot_remove", 2131624134);
            setStaticInt(env, clazz, "hs__screenshot_upload_error_msg", 2131624135);
            setStaticInt(env, clazz, "hs__search_footer", 2131624136);
            setStaticInt(env, clazz, "hs__search_hint", 2131624137);
            setStaticInt(env, clazz, "hs__search_result_message", 2131624138);
            setStaticInt(env, clazz, "hs__search_result_title", 2131624139);
            setStaticInt(env, clazz, "hs__search_title", 2131624140);
            setStaticInt(env, clazz, "hs__select_a_question_message", 2131624141);
            setStaticInt(env, clazz, "hs__send_anyway", 2131624142);
            setStaticInt(env, clazz, "hs__send_msg_btn", 2131624143);
            setStaticInt(env, clazz, "hs__sending_fail_msg", 2131624144);
            setStaticInt(env, clazz, "hs__sending_msg", 2131624145);
            setStaticInt(env, clazz, "hs__ssl_handshake_error", 2131624146);
            setStaticInt(env, clazz, "hs__ssl_peer_unverified_error", 2131624147);
            setStaticInt(env, clazz, "hs__starting_download", 2131624148);
            setStaticInt(env, clazz, "hs__submit_conversation_btn", 2131624149);
            setStaticInt(env, clazz, "hs__tap_to_retry", 2131624150);
            setStaticInt(env, clazz, "hs__try_again_msg", 2131624151);
            setStaticInt(env, clazz, "hs__user_failed_message_voice_over", 2131624152);
            setStaticInt(env, clazz, "hs__user_sending_message_voice_over", 2131624153);
            setStaticInt(env, clazz, "hs__user_sent_message_voice_over", 2131624154);
            setStaticInt(env, clazz, "hs__user_setup_retry_description", 2131624155);
            setStaticInt(env, clazz, "hs__username_blank_error", 2131624156);
            setStaticInt(env, clazz, "hs__username_hint", 2131624157);
            setStaticInt(env, clazz, "override_in_project", 2131624238);

            DM_LOGI("SET FOR 0:com.supercell.titan.R$string...");
            break;
        }
        case 1: {
            setStaticInt(env, clazz, "APP_STORE", 2);
            setStaticInt(env, clazz, "APP_STORE_AMAZON", 28);
            setStaticInt(env, clazz, "APP_STORE_CAFE_BAZAAR", 29);
            setStaticInt(env, clazz, "APP_STORE_GOOGLE_PLAY", 2);
            setStaticInt(env, clazz, "APP_STORE_GOOGLE_PLAY_INSTANT_APP", 35);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_360", 3);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_4399", 20);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_91", 12);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_ANZHI", 15);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_APP_CHINA", 21);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_AZSC", 17);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_BAIDU", 19);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_BAIDU_DUOKU", 5);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_BAIDU_TIEBA", 18);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_BILIBILI", 38);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_CHINA_MOBILE", 23);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_CHINA_TELECOM", 24);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_CHINA_UNICOM", 25);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_COOLPAD", 32);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_DOWNJOY", 9);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_EWAN", 36);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_GIONEE", 31);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_GUOPAN", 39);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_HUAWEI", 8);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_JINLI", 40);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_KINGSOFT", 26);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_LANDING_PAGE", 14);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_LENOVO", 11);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_MEIZU", 33);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_MHA", 30);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_OPPO", 7);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_TENCENT_YINGYONGBAO", 13);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_UC", 4);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_VIVO", 16);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_WANDOUJIA", 10);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_XIAOMI", 6);
            setStaticInt(env, clazz, "APP_STORE_KUNLUN_YOUKU", 22);
            setStaticInt(env, clazz, "APP_STORE_SAMSUNG", 27);
            setStaticInt(env, clazz, "APP_STORE_TENCENT", 34);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO", 37);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_360", 55);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_4399", 53);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_BAIDU", 59);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_BILIBILI", 51);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_COOLPAD", 50);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_GAMERSKY_COM", 69);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_GIONEE", 49);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_GUOPAN", 56);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_GUOPAN_LIULIU", 60);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_HAOYOU", 43);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_HISENSE", 61);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_HTC", 62);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_HUAWEI", 44);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_LENOVO", 48);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_MEITU", 63);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_MEIZU", 47);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_MGTV", 64);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_NUBIA", 58);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_OPPO", 45);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_RENRENGAME", 65);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_SAMSUNG", 66);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_SMARTISAN_UNION", 67);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_SOGOU_COM", 68);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_TAPTAP", 42);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_TT", 57);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_TZSY", 70);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_UC", 54);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_VIVO", 46);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_XIAOMI", 52);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_YOUZU", 41);
            setStaticInt(env, clazz, "APP_STORE_YOOZOO_YUEWEN", 71);
            setStaticBoolean(env, clazz, "CHINESE_BUILD", false);
            setStaticBoolean(env, clazz, "KUNLUN_BUILD", false);
            setStaticBoolean(env, clazz, "TENCENT_BUILD", false);
            setStaticBoolean(env, clazz, "USING_GOOGLE_PLAY", true);
            setStaticBoolean(env, clazz, "YOOZOO_BUILD", false);
            DM_LOGI("SET FOR 1:com.supercell.titan.TitanConfig...");
            break;
        }
        case 2: { // com.supercell.titan.HelpshiftTitan

            // Integer fields
            setStaticInt(env, clazz, "ORIENTATION_LANDSCAPE", 2);
            setStaticInt(env, clazz, "ORIENTATION_PORTRAIT", 1);
            setStaticInt(env, clazz, "ORIENTATION_UNSPECIFIED", 0);
            setStaticInt(env, clazz, "cachedNotificationCount", 0);
            setStaticInt(env, clazz, "mContactUsMode", 2);

            // HashMap fields
            jclass hashMapClass = (*env)->FindClass(env, "java/util/HashMap");
            if (hashMapClass == NULL) break;

            jmethodID hashMapCtor = (*env)->GetMethodID(env, hashMapClass, "<init>", "()V");
            if (hashMapCtor == NULL) {
                (*env)->DeleteLocalRef(env, hashMapClass);
                break;
            }

            jobject config_map = (*env)->NewObject(env, hashMapClass, hashMapCtor);
            jobject customIssueFields_map = (*env)->NewObject(env, hashMapClass, hashMapCtor);
            jobject customMetadata_map = (*env)->NewObject(env, hashMapClass, hashMapCtor);
            jobject dataTypeMapping_map = (*env)->NewObject(env, hashMapClass, hashMapCtor);
            jobject internalCustomIssueFields_map = (*env)->NewObject(env, hashMapClass,
                                                                      hashMapCtor);

            setStaticObject(env, clazz, "config", config_map, "Ljava/util/HashMap;");
            setStaticObject(env, clazz, "customIssueFields", customIssueFields_map,
                            "Ljava/util/HashMap;");
            setStaticObject(env, clazz, "customMetadata", customMetadata_map,
                            "Ljava/util/HashMap;");
            setStaticObject(env, clazz, "dataTypeMapping", dataTypeMapping_map,
                            "Ljava/util/HashMap;");
            setStaticObject(env, clazz, "internalCustomIssueFields", internalCustomIssueFields_map,
                            "Ljava/util/HashMap;");

            // String[] fields
            jclass stringClass = (*env)->FindClass(env, "java/lang/String");
            if (stringClass == NULL) {
                // Cleanup HashMap refs before breaking
                (*env)->DeleteLocalRef(env, hashMapClass);
                (*env)->DeleteLocalRef(env, config_map);
                (*env)->DeleteLocalRef(env, customIssueFields_map);
                (*env)->DeleteLocalRef(env, customMetadata_map);
                (*env)->DeleteLocalRef(env, dataTypeMapping_map);
                (*env)->DeleteLocalRef(env, internalCustomIssueFields_map);
                break;
            }

            jobjectArray emptyStringArray = (*env)->NewObjectArray(env, 0, stringClass, NULL);
            setStaticObject(env, clazz, "customTags", emptyStringArray, "[Ljava/lang/String;");
            setStaticObject(env, clazz, "withTagsMatching", emptyStringArray,
                            "[Ljava/lang/String;");

            // Boolean fields
            setStaticBoolean(env, clazz, "enableFullPrivacy", JNI_FALSE);
            setStaticBoolean(env, clazz, "installCalled", JNI_FALSE);
            setStaticBoolean(env, clazz, "isVisible", JNI_FALSE);
            setStaticBoolean(env, clazz, "opened", JNI_FALSE);

            // String field
            setStaticObject(env, clazz, "mInitialUserMessage", NULL, "Ljava/lang/String;");

            // Cleanup local references
            (*env)->DeleteLocalRef(env, hashMapClass);
            (*env)->DeleteLocalRef(env, config_map);
            (*env)->DeleteLocalRef(env, customIssueFields_map);
            (*env)->DeleteLocalRef(env, customMetadata_map);
            (*env)->DeleteLocalRef(env, dataTypeMapping_map);
            (*env)->DeleteLocalRef(env, internalCustomIssueFields_map);
            (*env)->DeleteLocalRef(env, stringClass);
            (*env)->DeleteLocalRef(env, emptyStringArray);

            DM_LOGI("SET FOR 2:com.supercell.titan.HelpshiftTitan...");
            break;
            DM_LOGI("SET FOR 2:com.supercell.titan.HelpshiftTitan...");
            break;
        }
        case 3: { // com.supercell.titan.ApplicationUtilBase

            // String constants
            setStaticString(env, clazz, "PREFERENCES_NEW", "storage_new");
            setStaticString(env, clazz, "PREFERENCES_OLD", "storage");

            // Object fields
            setStaticObject(env, clazz, "m_cachedAndroidId", NULL, "Ljava/lang/String;");
            setStaticObject(env, clazz, "memInfo", NULL, "Landroid/os/Debug$MemoryInfo;");
            setStaticInt(env, clazz, "totalMemory", -267522035);

            DM_LOGI("SET FOR 3:com.supercell.titan.ApplicationUtilBase...");
            break;
        }
        case 4: { // com.supercell.titan.QuagoManager

            // Boolean fields
            setStaticBoolean(env, clazz, "quagoInitialized", JNI_FALSE);
            setStaticBoolean(env, clazz, "quagoReleased", JNI_FALSE);

            // instance = new QuagoManager()
            jclass quagoMgrClass = (*env)->FindClass(env, "com/supercell/titan/QuagoManager");
            if (quagoMgrClass != NULL) {
                jmethodID ctor = (*env)->GetMethodID(env, quagoMgrClass, "<init>", "()V");
                if (ctor != NULL) {
                    jobject quagoMgrInstance = (*env)->NewObject(env, quagoMgrClass, ctor);
                    setStaticObject(env, clazz, "instance", quagoMgrInstance,
                                    "Lcom/supercell/titan/QuagoManager;");
                    (*env)->DeleteLocalRef(env, quagoMgrInstance);
                }
                (*env)->DeleteLocalRef(env, quagoMgrClass);
            }

            // gameApp = new com.supercell.titan.GameApp()
            jclass gameAppClass = (*env)->FindClass(env, "com/supercell/titan/GameApp");
            if (gameAppClass != NULL) {
                jmethodID ctor = (*env)->GetMethodID(env, gameAppClass, "<init>", "()V");
                if (ctor != NULL) {
                    jobject gameAppInstance = (*env)->NewObject(env, gameAppClass, ctor);
                    setStaticObject(env, clazz, "gameApp", gameAppInstance,
                                    "Lcom/supercell/titan/GameApp;");
                    (*env)->DeleteLocalRef(env, gameAppInstance);
                }
                (*env)->DeleteLocalRef(env, gameAppClass);
            }

            DM_LOGI("SET FOR 4:com.supercell.titan.QuagoManager...");
            break;
        }

        case 5: { // com.supercell.titan.GL2JNISurfaceView$TouchEvent

            setStaticInt(env, clazz, "TYPE_CANCEL", 3);
            setStaticInt(env, clazz, "TYPE_DOWN", 0);
            setStaticInt(env, clazz, "TYPE_MOVE", 2);
            setStaticInt(env, clazz, "TYPE_UP", 1);

            DM_LOGI("SET FOR 5:com.supercell.titan.GL2JNISurfaceView$TouchEvent...");
            break;
        }

        case 6: { // com.supercell.titan.quago.BuildConfig

            setStaticString(env, clazz, "BUILD_TYPE", "release");
            setStaticBoolean(env, clazz, "DEBUG", JNI_FALSE);
            setStaticString(env, clazz, "LIBRARY_PACKAGE_NAME", "com.supercell.titan.quago");

            // Create ArrayList with enabled features
            const char *features[] = {
                    "InAppReview",
                    "Quago",
                    "ScreenSizeChangeSupport",
                    "Promon",
                    "GoogleInstallTimeAssetPack"
            };
            jobject enabledList = createArrayList(env, features, 5);
            setStaticObject(env, clazz, "EnabledFeatures", enabledList, "Ljava/util/ArrayList;");
            (*env)->DeleteLocalRef(env, enabledList);

            // Set string constants a, b, c, d, e to match feature names
            setStaticString(env, clazz, "a", "InAppReview");
            setStaticString(env, clazz, "b", "Promon");
            setStaticString(env, clazz, "c", "Quago");
            setStaticString(env, clazz, "d", "GoogleInstallTimeAssetPack");
            setStaticString(env, clazz, "e", "ScreenSizeChangeSupport");

            DM_LOGI("SET FOR 6:com.supercell.titan.quago.BuildConfig...");
            break;
        }

        case 7: { // com.supercell.titan.GameApp

            // LOG_OUT_REASON_* and orientation ints
            setStaticInt(env, clazz, "LOG_OUT_REASON_ANTI_ADDICTION", 6);
            setStaticInt(env, clazz, "LOG_OUT_REASON_AUTH_EXPIRED", 2);
            setStaticInt(env, clazz, "LOG_OUT_REASON_CANCEL_QQ_AUTH", 7);
            setStaticInt(env, clazz, "LOG_OUT_REASON_CANCEL_WECHAT_AUTH", 8);
            setStaticInt(env, clazz, "LOG_OUT_REASON_DENY_WECHAT_AUTH", 9);
            setStaticInt(env, clazz, "LOG_OUT_REASON_LOGIN_TIMEOUT", 4);
            setStaticInt(env, clazz, "LOG_OUT_REASON_NOT_IN_WHITE_LIST", 5);
            setStaticInt(env, clazz, "LOG_OUT_REASON_SWITCH_USER", 3);
            setStaticInt(env, clazz, "LOG_OUT_REASON_UNKNOWN", 0);
            setStaticInt(env, clazz, "LOG_OUT_REASON_USER", 1);
            setStaticInt(env, clazz, "SHOW_STORE_PAGE_REQUEST_CODE", 990001);
            setStaticInt(env, clazz, "ScreenOrientation_LandscapeLeft", 4);
            setStaticInt(env, clazz, "ScreenOrientation_LandscapeRight", 8);
            setStaticInt(env, clazz, "ScreenOrientation_Portrait", 1);
            setStaticInt(env, clazz, "ScreenOrientation_PortraitUpsideDown", 2);

            // String fields
            setStaticString(env, clazz, "a", "this game");
            setStaticString(env, clazz, "b", "");

            // instance = new GameApp()
            jclass gameAppClass = (*env)->FindClass(env, "com/supercell/titan/GameApp");
            if (gameAppClass != NULL) {
                jmethodID ctor = (*env)->GetMethodID(env, gameAppClass, "<init>", "()V");
                if (ctor != NULL) {
                    jobject instanceObj = (*env)->NewObject(env, gameAppClass, ctor);
                    setStaticObject(env, clazz, "instance", instanceObj,
                                    "Lcom/supercell/titan/GameApp;");
                    (*env)->DeleteLocalRef(env, instanceObj);
                }
                (*env)->DeleteLocalRef(env, gameAppClass);
            }

            setStaticInt(env, clazz, "notificationIdCounter", 1758567658);

            // pendingAlarmTimes and pendingAlarms = new Vector()
            jclass vectorClass = (*env)->FindClass(env, "java/util/Vector");
            if (vectorClass != NULL) {
                jmethodID vectorCtor = (*env)->GetMethodID(env, vectorClass, "<init>", "()V");
                if (vectorCtor != NULL) {
                    jobject pendingAlarmTimes = (*env)->NewObject(env, vectorClass, vectorCtor);
                    jobject pendingAlarms = (*env)->NewObject(env, vectorClass, vectorCtor);

                    setStaticObject(env, clazz, "pendingAlarmTimes", pendingAlarmTimes,
                                    "Ljava/util/Vector;");
                    setStaticObject(env, clazz, "pendingAlarms", pendingAlarms,
                                    "Ljava/util/Vector;");

                    (*env)->DeleteLocalRef(env, pendingAlarmTimes);
                    (*env)->DeleteLocalRef(env, pendingAlarms);
                }
                (*env)->DeleteLocalRef(env, vectorClass);
            }

            DM_LOGI("SET FOR 7:com.supercell.titan.GameApp...");
            break;
        }

        case 8: { // com.supercell.titan.usercentrics.UsercentricsSDKKt

            setStaticInt(env, clazz, "BUTTON_LAYOUT_COLUMN", 1);
            setStaticInt(env, clazz, "BUTTON_LAYOUT_DEFAULT", 0);
            setStaticInt(env, clazz, "BUTTON_LAYOUT_GRID", 3);
            setStaticInt(env, clazz, "BUTTON_LAYOUT_ROW", 2);

            DM_LOGI("SET FOR 8:com.supercell.titan.usercentrics.UsercentricsSDKKt...");
            break;
        }

        case 9: { // com.supercell.titan.ApplicationUtil

            // cachedAdvertisingId = ""
            setStaticString(env, clazz, "cachedAdvertisingId", "");

            // cachedAdvertisingIdEnabled = false
            setStaticBoolean(env, clazz, "cachedAdvertisingIdEnabled", JNI_FALSE);

            // m_adInfoRetrieved = null (AtomicBoolean)
            setStaticObject(env, clazz, "m_adInfoRetrieved", NULL,
                            "Ljava/util/concurrent/atomic/AtomicBoolean;");

            // m_adInfo = null (AdvertisingIdClient.Info)
            setStaticObject(env, clazz, "m_adInfo", NULL,
                            "Lcom/google/android/gms/ads/identifier/AdvertisingIdClient$Info;");

            DM_LOGI("SET FOR 9:com.supercell.titan.ApplicationUtil...");
            break;
        }

        case 10: { // com.supercell.titan.R$drawable

            setStaticInt(env, clazz, "edit_text_style", 2131165303);
            setStaticInt(env, clazz, "ic_notification", 2131165328);
            setStaticInt(env, clazz, "info_box_bg", 2131165329);

            DM_LOGI("SET FOR 10:com.supercell.titan.R$drawable...");
            break;
        }
        case 11: { // com.supercell.clashroyale.R$string

            setStaticInt(env, clazz, "app_id", 2131623966);
            setStaticInt(env, clazz, "app_name", 2131623967);
            setStaticInt(env, clazz, "backup_api_key", 2131623969);
            setStaticInt(env, clazz, "cadpa_text", 2131623972);
            setStaticInt(env, clazz, "default_web_client_id", 2131624006);
            setStaticInt(env, clazz, "facebook_application_id", 2131624012);
            setStaticInt(env, clazz, "firebase_database_url", 2131624017);
            setStaticInt(env, clazz, "gcm_defaultSenderId", 2131624018);
            setStaticInt(env, clazz, "google_api_key", 2131624019);
            setStaticInt(env, clazz, "google_app_id", 2131624020);
            setStaticInt(env, clazz, "google_crash_reporting_api_key", 2131624021);
            setStaticInt(env, clazz, "google_storage_bucket", 2131624022);
            setStaticInt(env, clazz, "googleplay_clientid", 2131624023);
            setStaticInt(env, clazz, "project_id", 2131624241);
            setStaticInt(env, clazz, "sc_manifest_package_name", 2131624242);

            DM_LOGI("SET FOR 11:com.supercell.clashroyale.R$string...");
            break;
        }

        case 12: { // com.supercell.titan.NetworkInfo

            setStaticInt(env, clazz, "TRANSPORT_CELLULAR", 3);
            setStaticInt(env, clazz, "TRANSPORT_NONE", 1);
            setStaticInt(env, clazz, "TRANSPORT_OTHER", 2);
            setStaticInt(env, clazz, "TRANSPORT_UNKNOWN", 0);
            setStaticInt(env, clazz, "TRANSPORT_VPN", 5);
            setStaticInt(env, clazz, "TRANSPORT_WIFI", 4);

            DM_LOGI("SET FOR 12:com.supercell.titan.NetworkInfo...");
            break;
        }

        case 13: { // com.supercell.titan.BuildConfig

            setStaticString(env, clazz, "BUILD_TYPE", "release");
            setStaticBoolean(env, clazz, "DEBUG", JNI_FALSE);
            setStaticString(env, clazz, "FLAVOR", "googlePlayFirebaseClashroyale");
            setStaticString(env, clazz, "FLAVOR_game", "clashroyale");
            setStaticString(env, clazz, "FLAVOR_messaging", "firebase");
            setStaticString(env, clazz, "FLAVOR_store", "googlePlay");
            setStaticString(env, clazz, "LIBRARY_PACKAGE_NAME", "com.supercell.titan");
            setStaticInt(env, clazz, "PROMON_PROTECTION", 1);
            setStaticInt(env, clazz, "storeCode", 2);

            const char *features[] = {
                    "InAppReview",
                    "Quago",
                    "ScreenSizeChangeSupport",
                    "Promon",
                    "GoogleInstallTimeAssetPack"
            };
            jobject enabledList = createArrayList(env, features, 5);
            setStaticObject(env, clazz, "EnabledFeatures", enabledList, "Ljava/util/ArrayList;");
            (*env)->DeleteLocalRef(env, enabledList);

            setStaticString(env, clazz, "a", "ScreenSizeChangeSupport");
            setStaticString(env, clazz, "b", "Promon");
            setStaticString(env, clazz, "c", "Quago");
            setStaticString(env, clazz, "d", "GoogleInstallTimeAssetPack");
            setStaticString(env, clazz, "e", "InAppReview");

            DM_LOGI("SET FOR 13:com.supercell.titan.BuildConfig...");
            break;
        }

        case 14: { // com.supercell.clashroyale.BuildConfig

            setStaticString(env, clazz, "ADJUST_APP_TOKEN", "ehqmfimyiygw");
            setStaticString(env, clazz, "ADJUST_ENVIRONMENT", "sandbox");
            setStaticString(env, clazz, "APPLICATION_ID", "com.supercell.clashroyale");
            setStaticString(env, clazz, "BUILD_TYPE", "release");
            setStaticBoolean(env, clazz, "DEBUG", JNI_FALSE);
            setStaticString(env, clazz, "FLAVOR", "googlePlay");
            setStaticString(env, clazz, "QUAGO_APP_TOKEN", "cr2r39guebc0");
            setStaticString(env, clazz, "SERVER_ENVIRONMENT", "production");
            setStaticInt(env, clazz, "VERSION_CODE", 110548004);
            setStaticString(env, clazz, "VERSION_NAME", "110548004");

            DM_LOGI("SET FOR 14:com.supercell.clashroyale.BuildConfig...");
            break;
        }

        case 15: { // com.supercell.titan.TimeAlarm

            setStaticString(env, clazz, "DEFAULT_CHANNEL_ID", "default");
            setStaticString(env, clazz, "NOTIFICATION_CHANNEL_DESCRIPTION", "channelDesc");
            setStaticString(env, clazz, "NOTIFICATION_CHANNEL_ID", "channelId");
            setStaticString(env, clazz, "NOTIFICATION_CHANNEL_NAME", "channelName");
            setStaticString(env, clazz, "NOTIFICATION_COLOR", "color");
            setStaticString(env, clazz, "NOTIFICATION_ID", "id");
            setStaticString(env, clazz, "NOTIFICATION_IMAGE_URL", "imageURL");
            setStaticString(env, clazz, "NOTIFICATION_MESSAGE", "msg");
            setStaticString(env, clazz, "NOTIFICATION_SOUND", "sound");
            setStaticString(env, clazz, "NOTIFICATION_TITLE", "title");
            setStaticString(env, clazz, "NOTIFICATION_USER_ID", "userId");

            DM_LOGI("SET FOR 15:com.supercell.titan.TimeAlarm...");
            break;
        }

        case 16: { // com.supercell.clashroyale.R$drawable

            setStaticInt(env, clazz, "ic_launcher", 2131165320);
            setStaticInt(env, clazz, "ic_notification", 2131165328);
            setStaticInt(env, clazz, "layer_background", 2131165330);
            setStaticInt(env, clazz, "layer_foreground", 2131165331);

            DM_LOGI("SET FOR 16:com.supercell.clashroyale.R$drawable...");
            break;
        }

        case 17: { // com.supercell.titan.R$style

            setStaticInt(env, clazz, "KeyboardDialog", 2131689748);
            setStaticInt(env, clazz, "KeyboardDialogAnimation", 2131689749);
            setStaticInt(env, clazz, "invisibleText", 2131690570);
            setStaticInt(env, clazz, "largeEdittextText", 2131690571);

            DM_LOGI("SET FOR 17:com.supercell.titan.R$style...");
            break;
        }

        case 18: { // com.supercell.titan.R$layout

            setStaticInt(env, clazz, "main", 2131427388);

            DM_LOGI("SET FOR 18:com.supercell.titan.R$layout...");
            break;
        }

        case 19: { // com.supercell.titan.usercentrics.BuildConfig

            setStaticString(env, clazz, "BUILD_TYPE", "release");
            setStaticBoolean(env, clazz, "DEBUG", JNI_FALSE);
            setStaticString(env, clazz, "LIBRARY_PACKAGE_NAME", "com.supercell.titan.usercentrics");

            const char *features[] = {
                    "InAppReview", "Quago", "ScreenSizeChangeSupport", "Promon",
                    "GoogleInstallTimeAssetPack"
            };
            jobject enabledList = createArrayList(env, features, 5);
            setStaticObject(env, clazz, "EnabledFeatures", enabledList, "Ljava/util/ArrayList;");
            (*env)->DeleteLocalRef(env, enabledList);

            DM_LOGI("SET FOR 19:com.supercell.titan.usercentrics.BuildConfig...");
            break;
        }

        case 20: { // com.supercell.titan.usercentrics.UsercentricsSDK

            setStaticString(env, clazz, "TAG", "UsercentricsSDK");

            jobject comp = getCompanionObject(env,
                                              "com/supercell/titan/usercentrics/UsercentricsSDK",
                                              "Companion",
                                              "Lcom/supercell/titan/usercentrics/UsercentricsSDK$Companion;"
            );
            setStaticObject(env, clazz, "Companion", comp,
                            "Lcom/supercell/titan/usercentrics/UsercentricsSDK$Companion;");

            DM_LOGI("SET FOR 20:com.supercell.titan.usercentrics.UsercentricsSDK...");
            break;
        }

        case 21: { // com.supercell.titan.R$layout

            setStaticInt(env, clazz, "sc_info_box", 2131427464);

            DM_LOGI("SET FOR 21:com.supercell.titan.R$layout...");
            break;
        }
        case 22: { // com.supercell.titan.ThermalMonitor

            setStaticInt(env, clazz, "THERMAL_PREDICTION_NOT_AVAILABLE", 0);
            setStaticInt(env, clazz, "THERMAL_PREDICTION_NOT_INITIALIZED", 1);
            setStaticInt(env, clazz, "THERMAL_PREDICTION_INITIALIZED", 2);
            setStaticInt(env, clazz, "THERMAL_PREDICTION_ONGOING", 3);

            // instance = new ThermalMonitor()
            jclass thermalMonitorClass = (*env)->FindClass(env,
                                                           "com/supercell/titan/ThermalMonitor");
            if (thermalMonitorClass != NULL) {
                jmethodID ctor = (*env)->GetMethodID(env, thermalMonitorClass, "<init>", "()V");
                if (ctor != NULL) {
                    jobject instance = (*env)->NewObject(env, thermalMonitorClass, ctor);
                    setStaticObject(env, clazz, "instance", instance,
                                    "Lcom/supercell/titan/ThermalMonitor;");
                    (*env)->DeleteLocalRef(env, instance);
                }
                (*env)->DeleteLocalRef(env, thermalMonitorClass);
            }

            DM_LOGI("SET FOR 22:com.supercell.titan.ThermalMonitor...");
            break;
        }

        case 23: { // com.supercell.titan.PurchaseManagerGoogle

            setStaticInt(env, clazz, "PURCHASE_REQUEST_CODE", 10000004);
            setStaticString(env, clazz, "b", "");
            setStaticString(env, clazz, "c", "");

            DM_LOGI("SET FOR 23:com.supercell.titan.PurchaseManagerGoogle...");
            break;
        }

        case 24: { // com.supercell.titan.R$anim

            setStaticInt(env, clazz, "keyboard_dialog_fadein", 2130771999);
            setStaticInt(env, clazz, "keyboard_dialog_fadeout", 2130772000);

            DM_LOGI("SET FOR 24:com.supercell.titan.R$anim...");
            break;
        }

        case 25: { // com.supercell.titan.NativeDialogManager

            setStaticString(env, clazz, "VISIBLE_NATIVE_DIALOG_TAG", "NativeDialog");
            setStaticInt(env, clazz, "idCounter", 1000);
            setStaticObject(env, clazz, "currentDialog", NULL,
                            "Lcom/supercell/titan/NativeDialogManager;");
            setStaticInt(env, clazz, "dismissedButtonID", 0);
            setStaticInt(env, clazz, "dismissedDialogID", 0);
            setStaticBoolean(env, clazz, "doDialogDismiss", JNI_FALSE);

            // pendingDialogs = new Vector()
            jclass vectorClass = (*env)->FindClass(env, "java/util/Vector");
            if (vectorClass != NULL) {
                jmethodID vectorCtor = (*env)->GetMethodID(env, vectorClass, "<init>", "()V");
                if (vectorCtor != NULL) {
                    jobject emptyVector = (*env)->NewObject(env, vectorClass, vectorCtor);
                    setStaticObject(env, clazz, "pendingDialogs", emptyVector,
                                    "Ljava/util/Vector;");
                    (*env)->DeleteLocalRef(env, emptyVector);
                }
                (*env)->DeleteLocalRef(env, vectorClass);
            }

            DM_LOGI("SET FOR 25:com.supercell.titan.NativeDialogManager...");
            break;
        }

        case 26: { // com.supercell.titan.R$id

            setStaticInt(env, clazz, "InfoBox", 2131230725);
            setStaticInt(env, clazz, "infoBoxButton", 2131230988);
            setStaticInt(env, clazz, "infoBoxText", 2131230989);
            setStaticInt(env, clazz, "stage", 2131231182);

            DM_LOGI("SET FOR 26:com.supercell.titan.R$id...");
            break;
        }

        case 27: { // com.supercell.titan.ScInfoBox

            setStaticInt(env, clazz, "e", -1);

            DM_LOGI("SET FOR 27:com.supercell.titan.ScInfoBox...");
            break;
        }

        case 28: { // com.supercell.titan.usercentrics.UsercentricsInitializer

            setStaticString(env, clazz, "TAG", "UsercentricsInitializer");

            jobject comp = getCompanionObject(env,
                                              "com/supercell/titan/usercentrics/UsercentricsInitializer",
                                              "Companion",
                                              "Lcom/supercell/titan/usercentrics/UsercentricsInitializer$Companion;"
            );
            setStaticObject(env, clazz, "Companion", comp,
                            "Lcom/supercell/titan/usercentrics/UsercentricsInitializer$Companion;");

            DM_LOGI("SET FOR 28:com.supercell.titan.usercentrics.UsercentricsInitializer...");
            break;
        }

        case 29: { // com.supercell.titan.R$xml

            setStaticInt(env, clazz, "screenshots_paths", 2131820550);

            DM_LOGI("SET FOR 29:com.supercell.titan.R$xml...");
            break;
        }


        default:
            DM_LOGI("Couldnt find class implementation...");
            break;
    }
}

jbyteArray nzijsy_aC_a__getAppAttestationResponseToken(JNIEnv *env,
                                                       jobject reserved_obj_arg,
                                                       jbyteArray client_session_arg) {
    return client_session_arg;
}

jboolean nzijsy_J_d__bollocks(JNIEnv *env, jobject reserved_obj_arg) {
    return JNI_TRUE;
}

void nzijsy_J_b_bollocks(JNIEnv *env, jobject reserved_obj_arg, jobject obj_arg) {
    (void) env;
    (void) obj_arg;
}

void nzijsy_az_a__spyware_1(JNIEnv *env, jobject reserved_obj_arg, jobject obj_arg) {
    (void) env;
    (void) obj_arg;
}

void nzijsy_az_b__spyware_1(JNIEnv *env, jobject reserved_obj_arg, jobject context_obj_arg,
                            jobject obj_arg) {
    (void) env;
    (void) context_obj_arg;
    (void) obj_arg;
}

void nzijsy_ay_a__spyware_0(JNIEnv *env,
                            jobject reserved_obj_arg,
                            jclass clazz_arg,
                            jint code_arg) {
    (void) env;
    (void) clazz_arg;
}

// ================== [ref-save] ==================

static inline __attribute__((always_inline)) unsigned char *decode_nzijsy_aq__9(void) {
    size_t __hc_len = 10u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDE5CDA9A5E9B984BULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F21ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_nzijsy_ay__9(void) {
    size_t __hc_len = 10u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDE5CDA9A5E9B984BULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F29ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_nzijsy_aC__9(void) {
    size_t __hc_len = 10u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDE5CDA9A5E9B984BULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F33ULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_nzijsy_J__8(void) {
    size_t __hc_len = 9u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDE5CDA9A5E9B928BULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    buf[8] = 0x0;
    return buf;
}

static inline __attribute__((always_inline)) unsigned char *decode_nzijsy_az__9(void) {
    size_t __hc_len = 10u;
    unsigned char *buf = (unsigned char *) mallocM(__hc_len);
    if (!buf) return NULL;
    volatile unsigned long long __hc_tmpA, __hc_tmpB;
    /* block 0 */
    __hc_tmpA = 0xB820E8334C6CEA63ULL;
    __hc_tmpA = (((((__hc_tmpA * 0x000000009D5E4B49ULL) >> 38) |
                   ((__hc_tmpA * 0x000000009D5E4B49ULL) << 26)) & 0xFFFFFFFFFFFFFFFFULL) ^
                 0x0000000036970A05ULL);
    __hc_tmpB = 0xDE5CDA9A5E9B984BULL;
    __hc_tmpB = ((((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) << 36) |
                  ((((__hc_tmpB >> 50) | (__hc_tmpB << 14)) & 0xFFFFFFFFFFFFFFFFULL) >> 28)) &
                 0xFFFFFFFFFFFFFFFFULL);
    buf[0] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[1] = (unsigned char) ((__hc_tmpB >> 8) & 0xFFULL);
    buf[2] = (unsigned char) ((__hc_tmpB >> 16) & 0xFFULL);
    buf[3] = (unsigned char) ((__hc_tmpB >> 24) & 0xFFULL);
    buf[4] = (unsigned char) ((__hc_tmpB >> 32) & 0xFFULL);
    buf[5] = (unsigned char) ((__hc_tmpB >> 40) & 0xFFULL);
    buf[6] = (unsigned char) ((__hc_tmpB >> 48) & 0xFFULL);
    buf[7] = (unsigned char) ((__hc_tmpB >> 56) & 0xFFULL);
    /* block 1 */
    __hc_tmpA = 0x05BDE0C3780BB3C5ULL;
    __hc_tmpA = (((((__hc_tmpA + 0x0000000073A5E55CULL) << 26) |
                   ((__hc_tmpA + 0x0000000073A5E55CULL) >> 38)) & 0xFFFFFFFFFFFFFFFFULL) *
                 0x000000008DA8AF8BULL);
    __hc_tmpB = 0x8C3BC4456FD54F2CULL;
    __hc_tmpB = ((__hc_tmpB - 0x000000006F325DD1ULL) ^ __hc_tmpA);
    buf[8] = (unsigned char) ((__hc_tmpB >> 0) & 0xFFULL);
    buf[9] = 0x0;
    return buf;
}

#define REGISTER_CLASS_AND_SAVE(env, class_path, out_global, jerr_arg) do { \
    jclass local = (*env)->FindClass(env, class_path);                      \
    if (!local) {                                                           \
        LOGE("Failed to find class %s", class_path);                        \
        return (s32t) jerr_arg;                                             \
    }                                                                       \
    out_global = (jclass)(*env)->NewGlobalRef(env, local);                  \
    (*env)->DeleteLocalRef(env, local);                                     \
} while(0)

jint dexmappings_save_ref(JavaVM *vm_arg) {
    s32t jerr = OBF_INT32(JNI_ERR);
    u32t jv = OBF_INT32(JNI_VERSION_1_6);

    JNIEnv *env = NULL;
    if ((*vm_arg)->GetEnv(vm_arg, (void **) &env, DEOBF_INT32(jv)) != JNI_OK) {
        return (s32t) DEOBF_INT32(jerr);
    }

    i8 *decoded_nzijsy_aq = (i8 *) decode_nzijsy_aq__9();
    i8 *decoded_nzijsy_ay = (i8 *) decode_nzijsy_ay__9();
    i8 *decoded_nzijsy_aC = (i8 *) decode_nzijsy_aC__9();
    i8 *decoded_nzijsy_J = (i8 *) decode_nzijsy_J__8();
    i8 *decoded_nzijsy_az = (i8 *) decode_nzijsy_az__9();

    jcode10();
    REGISTER_CLASS_AND_SAVE(env, "gobvpmes/aa", nzijsy_aq_class, jerr);
    jcode01();
    REGISTER_CLASS_AND_SAVE(env, "gobvpmes/x", nzijsy_ay_class, jerr);
    jcode07();
    REGISTER_CLASS_AND_SAVE(env, "gobvpmes/aJ", nzijsy_aC_class, jerr);
    jcode01();
    jcode05();
    REGISTER_CLASS_AND_SAVE(env, "gobvpmes/d", nzijsy_J_class, jerr);
    jcode01();
    jcode04();
    REGISTER_CLASS_AND_SAVE(env, "gobvpmes/aw", nzijsy_az_class, jerr);
    jcode01();
    jcode02();
    jcode03();

    secure_memory_wipe(decoded_nzijsy_aq, 9, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_nzijsy_ay, 9, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_nzijsy_aC, 9, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_nzijsy_J, 8, SECURE_MEMORY_WIPE_FREE);
    secure_memory_wipe(decoded_nzijsy_az, 9, SECURE_MEMORY_WIPE_FREE);

    if (!nzijsy_J_class || !nzijsy_aC_class || !nzijsy_aq_class || !nzijsy_az_class ||
        !nzijsy_ay_class) {
        jcode07();
        return demolisher_stackoverflower();
        return 0;
    }

    jcode01();

    return (jint) 67;
}
