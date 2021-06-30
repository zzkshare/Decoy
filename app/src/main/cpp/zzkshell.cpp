#include "common.h"
using namespace std;

std::string exec(const char *);

std::string exec_with_root(const char *);

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_zznative_MainActivity_execJNI(
jstring decoy(
        JNIEnv *env,
        jclass obj,
        jstring str) {
    const char *cmd = env->GetStringUTFChars(str, nullptr);
    // int ret = system(cmd.c_str());
    std::string ret = exec(cmd);

    // char* ret_s = std::to_string(ret);
    return env->NewStringUTF(ret.c_str());
}

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_zznative_MainActivity_execJNIWithRoot(
jstring decoy0(
        JNIEnv *env,
        jclass obj,
        jstring str) {
    const char *cmd = env->GetStringUTFChars(str, nullptr);
    std::string ret = exec_with_root(cmd);
    return env->NewStringUTF(ret.c_str());
}

//extern "C" JNIEXPORT jstring JNICALL
//Java_com_example_zznative_MainActivity_getHostByName(
jstring decoy1(
        JNIEnv *env,
        jclass  obj,
        jstring str) {
    const char *name = env->GetStringUTFChars(str, nullptr);

    struct hostent *host = gethostbyname(name);
    if (!host) {
        return env->NewStringUTF("-1");
    }
    char *ip = inet_ntoa(*(struct in_addr *) host->h_addr_list[0]);

    return env->NewStringUTF(ip);
}

std::string exec(const char *cmd) {
    std::array<char, 128> buffer;
    std::string result;
    // std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    char cmd_with_err[255] = {0};
    sprintf(cmd_with_err, "%s 2>&1", cmd);

    auto pipe = popen(cmd_with_err, "r");

    if (!pipe) {
        return "error";
    }

    while (!feof(pipe)) {
        if (fgets(buffer.data(), 128, pipe) != nullptr)
            result += buffer.data();
    }
    auto rc = pclose(pipe);

    return result;
}

std::string exec_with_root(const char *cmd) {
    std::array<char, 128> buffer;
    std:string result;

    char cmd_with_err[255] = {0};
    sprintf(cmd_with_err, "su -c %s 2>&1", cmd);

    auto pipe = popen(cmd_with_err, "r");

    if (!pipe) return "error";
    while (!feof(pipe)) {
        LOGD("open start %s", cmd);
        if (fgets(buffer.data(), 128, pipe) != nullptr){
            result += buffer.data();
            LOGD("get result");
        }
        LOGD("open end");
    }
    auto rc = pclose(pipe);
    return result;
}

int main() {
    string a = exec("whoami");
    printf("output: %s", a.c_str());
    return 0;
}