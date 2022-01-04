#include "common.h"

jstring base64_encode(JNIEnv *env, jclass obj, jstring str) {
    const char *data = env->GetStringUTFChars(str, NULL);
    int len = sizeof(data);
    int base64_len = (len / 3 + 1) * 4;
    char *out = (char *) malloc(base64_len);
    base64_enc(data, out);
    jstring ret = env->NewStringUTF(out);
    env->ReleaseStringUTFChars(str, data);
    free(out);
    return ret;
}

jstring base64_decode(JNIEnv *env, jclass obj, jstring str) {
    const char *data = env->GetStringUTFChars(str, NULL);
    int len = sizeof(data);
    int base64_len = (len / 3 + 1) * 4;
    char *out = (char *) malloc(base64_len);
    base64_dec(data, out);
    jstring ret = env->NewStringUTF(out);
    env->ReleaseStringUTFChars(str, data);
    free(out);
    return ret;
}

jstring rc4(JNIEnv *env, jclass obj, jstring key, jbyteArray content) {
    jsize num_bytes = env->GetArrayLength(content);
    jbyte *jdata = env->GetByteArrayElements(content, NULL);
    const char *c_key = env->GetStringUTFChars(key, NULL);
//    char *data = reinterpret_cast<char *>(jdata);
    char *data = (char *)(malloc(static_cast<size_t>(num_bytes)));
    memcpy(data, jdata, num_bytes);
    data[num_bytes] = 0;
    unsigned char S[255] = {0};
    char out[1024] = {0};
    rc4_init(S, c_key);
    rc4_enc(S, data, strlen(data));
    base64_enc(data, out);
    jstring ret = env->NewStringUTF(out);
    env->ReleaseByteArrayElements(content, jdata, 0);
    env->ReleaseStringUTFChars(key, c_key);
    free(data);
    return ret;
}
