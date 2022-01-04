#include "common.h"

using namespace std;
void rc4_init(unsigned char *S, const char *K) {
    unsigned char T[256] = {0};
    int keylen = strlen(K);
    int i = 0, j = 0;
    for (i = 0; i < 256; ++i) {
        S[i] = i;
        T[i] = K[i % keylen];
    }
    for (i = 0; i < 256; ++i) {
        j = (j + S[i] + T[i]) % 256;
        swap(S[i], S[j]);
    }
}

void rc4_enc(unsigned char* S, char *data, int length) {
    int i=0, j=0, t=0;
    int index=0;
    while (index<length) {
        i = (i+1) % 256;
        j = (j+S[i]) % 256;
        swap(S[i], S[j]);
        t = (S[i] + S[j]) % 256;
        data[index++] ^= S[t];
    }
}
