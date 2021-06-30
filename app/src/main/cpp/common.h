#include <jni.h>
#include <string>
#include <sys/ptrace.h>
#include <array>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <android/log.h>

#define TAG "zznative" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


jstring decoy(JNIEnv *env, jclass obj, jstring str);
jstring decoy0(JNIEnv *env, jclass obj, jstring str);
jstring decoy1(JNIEnv *env, jclass obj, jstring str);

static JNINativeMethod method_table[] = {
        {"execJNI", "(Ljava/lang/String;)Ljava/lang/String;",(jstring)decoy},
        {"execJNIWithRoot", "(Ljava/lang/String;)Ljava/lang/String;",(jstring)decoy0},
        {"getHostByName", "(Ljava/lang/String;)Ljava/lang/String;",(jstring)decoy1}
};
