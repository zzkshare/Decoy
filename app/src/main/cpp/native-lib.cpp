#include "common.h"

#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

   
static int registerMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods) {
	LOGD("enter registerMethods");
	jclass clazz = env->FindClass(className);
	if (clazz == nullptr) {
		LOGD("enter nullptr");
		return JNI_FALSE;
	}
	if (env->RegisterNatives(clazz, gMethods, numMethods)) {
		LOGD("enter RegisterNatives");
		return JNI_FALSE;
	}
	return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
//	ptrace(PTRACE_TRACEME, 0,0,0);
	LOGD("enter JNI_OnLoad");
	JNIEnv *env = nullptr;
	if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
		LOGD("enter JNI_NOT_OK");
		return JNI_ERR;
	}
	if (!registerMethods(env, "top/zzkshare/decoy/utils/JNIInterfaceManager", method_table, NELEM(method_table))) {
		LOGD("enter register err");
		return JNI_ERR;
	}
	return JNI_VERSION_1_6;

}
