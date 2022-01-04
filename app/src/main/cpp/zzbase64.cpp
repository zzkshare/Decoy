#include "common.h"

static const char base64_code_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
                                         'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                         'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                                         'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                         'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
                                         '8', '9', '+', '/'};


// 基于ascii码表 对应相应的 base64 的值 62->+ , 63->/
static const int base64_decode_table[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -2, -1, -1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static const char base64_pad = '=';

#define MASK_BYTE_HIGH_6BIT 0xFC
#define MASK_BYTE_LOW_6BIT 0x3F
#define MASK_BYTE_HIGH_4BIT 0xf0
#define MASK_BYTE_LOW_4BIT 0x0f
#define MASK_BYTE_HIGH_2BIT 0xC0
#define MASK_BYTE_LOW_2BIT 0x03

void base64_enc(const char *data, char *out) {
    int len_data = (int) strlen(data);
    int index = 0;
    char pre_c = 0;
    int offset = 0;
    for (int i = 0; i < len_data; i++) {
        switch (i % 3) {
            case 0:
                offset = (data[i] & MASK_BYTE_HIGH_6BIT) >> 2;
                out[index++] = base64_code_table[offset];
                break;
            case 1:
                offset = (pre_c & MASK_BYTE_LOW_2BIT) << 4 | (data[i] & MASK_BYTE_HIGH_4BIT) >> 4;
                out[index++] = base64_code_table[offset];
                break;
            case 2:
                offset = (pre_c & MASK_BYTE_LOW_4BIT) << 2 | (data[i] & MASK_BYTE_HIGH_2BIT) >> 6;
                out[index++] = base64_code_table[offset];
                out[index++] = base64_code_table[data[i] & MASK_BYTE_LOW_6BIT];
                break;
            default:
                break;
        }
        pre_c = data[i];
    }
    if (len_data % 3 == 1) {
        out[index++] = base64_code_table[(pre_c & MASK_BYTE_LOW_2BIT) << 4];
        out[index++] = base64_pad;
        out[index++] = base64_pad;
    } else if (len_data % 3 == 2) {
        out[index++] = base64_code_table[(pre_c & MASK_BYTE_LOW_4BIT) << 2];
        out[index++] = base64_pad;
    }
    out[index++] = 0;
}

/*
 *      |    M      |     a     |     n     |     M     |           |           |
 *      |01|00|11|01|01|10|00|01|01|10|11|10|01|10|00|01|00|00|00|00|00|00|00|00|
 *      |    T   |    W   |   F    |    u   |    T   |    Q   |   =    |  =
 */
void base64_dec(const char *enc_data, char *out) {
    int len_data = (int)strlen(enc_data);
    if (len_data % 4 != 0) {
        out[0] = '\0';
        return;
    }
    int index = 0;
    int offset = 0;
    while (offset+4 < len_data) {
        out[index] = (base64_decode_table[enc_data[offset]] & MASK_BYTE_LOW_6BIT) << 2 | (base64_decode_table[enc_data[offset+1]] & MASK_BYTE_HIGH_4BIT) >> 4;
        out[index+1] = (base64_decode_table[enc_data[offset+1]] & MASK_BYTE_LOW_4BIT) << 4 | (base64_decode_table[enc_data[offset+2]] & MASK_BYTE_HIGH_6BIT) >> 2;
        out[index+2] = (base64_decode_table[enc_data[offset+2]] & MASK_BYTE_LOW_2BIT) << 6 | (base64_decode_table[enc_data[offset+3]] & MASK_BYTE_LOW_6BIT);
        index+=3;
        offset+=4;
    }
    out[index++]  = (base64_decode_table[enc_data[offset]] & MASK_BYTE_LOW_6BIT) << 2 | (base64_decode_table[enc_data[++offset]] & MASK_BYTE_HIGH_4BIT) >> 4;
    if (enc_data[offset+1] == '=') {
        out[index++] = '\0';
        return;
    }
    out[index++] = (base64_decode_table[enc_data[offset]] & MASK_BYTE_LOW_4BIT) << 4 | (base64_decode_table[enc_data[++offset]] & MASK_BYTE_HIGH_6BIT) >> 2;
    if (enc_data[offset+1] == '=') {
        out[index++] = '\0';
        return;
    }
    out[index++] = (base64_decode_table[enc_data[offset]] & MASK_BYTE_LOW_2BIT) << 6 | (base64_decode_table[enc_data[++offset]] & MASK_BYTE_LOW_6BIT);
    out[index++] = '\0';
    return;
}




//extern "C"
//JNIEXPORT jstring JNICALL
//Java_com_kanxue_base64_MainActivity_base64enc(JNIEnv *env, jobject thiz, jstring str) {
//    const char *data = env->GetStringUTFChars(str,NULL);
//    char out[100] = {0};
//    base64_enc(data,out);
//    env->ReleaseStringUTFChars(str,data);
//    return env->NewStringUTF(out);
//}
