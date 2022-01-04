jstring base64_encode(JNIEnv *env, jclass obj, jstring str);
jstring base64_decode(JNIEnv *env, jclass obj, jstring str);

jstring rc4(JNIEnv *env, jclass obj, jstring key, jbyteArray content);

void base64_enc(const char *data, char *out);
void base64_dec(const char *enc_data, char *out);

void rc4_init(unsigned char *S, const char *K);
void rc4_enc(unsigned char* S, char *data, int length);