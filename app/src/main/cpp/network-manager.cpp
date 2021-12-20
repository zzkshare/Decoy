#include "common.h"

#define BUFFER_SIZE 10240
#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"
#define HTTP_GET "GET /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n\r\n"

#define PROXY_ENABLE 1
#define PROXY_CONNECT "CONNECT %s:%d HTTP/1.1\r\n\r\n"

static int http_tcpclient_create(const char* host, int port) {
    struct hostent* he;
    struct sockaddr_in server_addr = { 0 };
    int socket_fd;
    if ((he = gethostbyname(host)) == NULL) {
        return -1;
    }

    server_addr.sin_family = AF_INET;
#if PROXY_ENABLE
    struct hostent* proxy_host;
//    if ((proxy_host = gethostbyname("charles.176971.xyz")) == NULL) {
//        server_addr.sin_addr = *((struct in_addr*)he->h_addr);
//    }
//    server_addr.sin_addr = *((struct in_addr*)proxy_host->h_addr);
    server_addr.sin_addr.s_addr = inet_addr("172.16.0.45");
    server_addr.sin_port = htons(8888);
#else
    server_addr.sin_addr = *((struct in_addr*)he->h_addr);
    server_addr.sin_port = htons(port);
#endif
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return -1;
    }

    if (connect(socket_fd, (struct sockaddr*) & server_addr, sizeof(struct sockaddr)) == -1) {
        return -1;
    }

    return socket_fd;
}
/*
 * 关闭连接
 * */
static void http_tcpclient_close(int socket) {
    close(socket);
}
/*
 * 解析URL
 * 分理出host,part 和 path
 * */
static int http_parse_url(const char* url, char* host, char* file_path, int* port)
{
    char* no_protocl_url, * path;
    bool isHttps = false;
    int host_len = 0;
    if (!url || !host || !file_path || !port) {
        return -1;
    }

    no_protocl_url = (char*)url;
    if (!strncmp(no_protocl_url, "http://", strlen("http://"))) {
        no_protocl_url += strlen("http://");  //  偏移掉协议头
    } else if (!strncmp(no_protocl_url, "https://", strlen("https://"))) {
        no_protocl_url += strlen("https://");  //  偏移掉协议头
        isHttps = true;
    } else {
        return -1;
    }

    path = strchr(no_protocl_url, '/');
    if (path) {
        host_len = static_cast<int>(strlen(no_protocl_url) - strlen(path));
        memcpy(host, no_protocl_url, host_len);
        host[host_len] = '\0';
        if (*(path + 1)) {
            memcpy(file_path, path + 1, strlen(path) - 1);
            file_path[strlen(path) - 1] = '\0';
        }
    } else {
        memcpy(host, no_protocl_url, strlen(no_protocl_url));
        host[strlen(no_protocl_url)] = '\0';
    }
    //get host and ip
    no_protocl_url = strchr(host, ':');
    if (no_protocl_url) {
        *no_protocl_url++ = '\0';
        *port = atoi(no_protocl_url);
    }
    else {
        *port = isHttps ? 443 : 80;
    }

    return 0;
}


static int http_tcpclient_recv(int socket, char* lpbuff) {
    int recvnum = 0;

    recvnum = static_cast<int>(recv(socket, lpbuff, BUFFER_SIZE * 4, 0));

    return recvnum;
}

static int http_tcpclient_send(int socket, char* buff, int size) {
    int sent = 0, tmpres = 0;

    while (sent < size) {
        tmpres = static_cast<int>(send(socket, buff + sent, size - sent, 0));
        if (tmpres == -1) {
            return -1;
        }
        sent += tmpres;
    }
    return sent;
}

static void connect_proxy(int sockfd, char* host, int port) {
    char lpbuf[BUFFER_SIZE * 4] = { '\0' };
    sprintf(lpbuf, PROXY_CONNECT, host, port);
    send(sockfd, lpbuf, strlen(lpbuf), 0);
}

static char* http_parse_result(const char* lpbuf)
{
    char* ptmp = NULL;
    char* response = NULL;
    ptmp = (char*)strstr(lpbuf, "HTTP/1.1");
//    if (!ptmp) {
//        printf("http/1.1 not faind\n");
//        return NULL;
//    }
//    if (atoi(ptmp + 9) != 200) {
//        printf("result:\n%s\n", lpbuf);
//        return NULL;
//    }

    ptmp = (char*)strstr(lpbuf, "\r\n");
    if (!ptmp) {
        printf("ptmp is NULL\n");
        return NULL;
    }
    response = (char*)malloc(strlen(ptmp) + 1);
    if (!response) {
        printf("malloc failed \n");
        return NULL;
    }
    strcpy(response, ptmp + 4);
    return response;
}
/*
 * Post请求
 * */
char* http_post(const char* url, const char* post_str) {

    char post[BUFFER_SIZE] = { '\0' };
    int socket_fd = -1;
    char lpbuf[BUFFER_SIZE * 4] = { '\0' };
    char* ptmp;
    char host_addr[BUFFER_SIZE] = { '\0' };
    char file[BUFFER_SIZE] = { '\0' };
    int port = 0;
    int len = 0;
    char* response = NULL;

    if (!url || !post_str) {
        printf("      failed!\n");
        return NULL;
    }

    if (http_parse_url(url, host_addr, file, &port)) {
        printf("http_parse_url failed!\n");
        return NULL;
    }
    socket_fd = http_tcpclient_create(host_addr, port);

#if PROXY_ENABLE
    connect_proxy(socket_fd, host_addr, port);
    recv(socket_fd, lpbuf, BUFFER_SIZE, 0);
#endif
    if (socket_fd < 0) {
        printf("http_tcpclient_create failed\n");
        return NULL;
    }

    sprintf(lpbuf, HTTP_POST, file, host_addr, port, strlen(post_str), post_str);

    if (http_tcpclient_send(socket_fd, lpbuf, strlen(lpbuf)) < 0) {
        printf("http_tcpclient_send failed..\n");
        return NULL;
    }
    //printf("发送请求:\n%s\n",lpbuf);

    /*it's time to recv from server*/
    if (http_tcpclient_recv(socket_fd, lpbuf) <= 0) {
        printf("http_tcpclient_recv failed\n");
        return NULL;
    }

    http_tcpclient_close(socket_fd);

    return lpbuf; //http_parse_result(lpbuf);
}
/*
 * Get请求
 * */
//char* http_get(const char* url)
//{
//
//    char post[BUFFER_SIZE] = { '\0' };
//    int socket_fd = -1;
//    char lpbuf[BUFFER_SIZE * 4] = { '\0' };
//    char* ptmp;
//    char host_addr[BUFFER_SIZE] = { '\0' };
//    char file_path[BUFFER_SIZE] = {'\0' };
//
//    int port = 0;
//    int len = 0;
//
//    if (!url) {
//        printf("      failed!\n");
//        return NULL;
//    }
//
//    if (http_parse_url(url, host_addr, file_path, &port)) {
//        printf("http_parse_url failed!\n");
//        return NULL;
//    }
//    //printf("host_addr : %s\tfile_path:%s\t,%d\n",host_addr,file_path,port);
//
//    socket_fd = http_tcpclient_create(host_addr, port);
//    if (socket_fd < 0) {
//        printf("http_tcpclient_create failed\n");
//        return NULL;
//    }
//
//    sprintf(lpbuf, HTTP_GET, file_path, host_addr, port);
//
//    if (http_tcpclient_send(socket_fd, lpbuf, strlen(lpbuf)) < 0) {
//        printf("http_tcpclient_send failed..\n");
//        return NULL;
//    }
//    //  printf("发送请求:\n%s\n",lpbuf);
//
//    if (http_tcpclient_recv(socket_fd, lpbuf) <= 0) {
//        printf("http_tcpclient_recv failed\n");
//        return NULL;
//    }
//    http_tcpclient_close(socket_fd);
//    return http_parse_result(lpbuf);
//}

extern "C" JNIEXPORT jstring JNICALL
Java_top_zzkshare_decoy_utils_JNIInterfaceManager_sendPostByNative(
//jstring native_post(
        JNIEnv *env,
        jclass obj,
        jstring url,
        jstring body) {
    const char *c_url = env->GetStringUTFChars(url, nullptr);
    const char *c_body = env->GetStringUTFChars(body, nullptr);

    char* result = http_post(c_url, c_body);
    std::string ret = result == NULL ? "" : result;
    return env->NewStringUTF(ret.c_str());
}


///*
// * 字符串转换
// * */
//char*   Jstring2CStr(JNIEnv*   env,   jstring   jstr)
//{
//    char*   rtn   =   NULL;
//    jclass   clsstring   =   (*]env)->FindClass(env,"java/lang/String");
//    jstring   strencode   =  (*env)->NewStringUTF(env,"GB2312");
//    jmethodID   mid   =   (*env)->GetMethodID(env,clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
//    jbyteArray   barr=   (jbyteArray)(*env)->CallObjectMethod(env,jstr,mid,strencode);
//    jsize   alen   =  (*env)->GetArrayLength(env,barr);
//    jbyte*   ba   =   (*env)->GetByteArrayElements(env,barr,JNI_FALSE);
//    if(alen   >   0)
//    {
//        rtn   =   (char*)malloc(alen+1);         //new   char[alen+1];
//        memcpy(rtn,ba,alen);
//        rtn[alen]=0;
//    }
//    (*env)->ReleaseByteArrayElements(env,barr,ba,0);
//
//    return rtn;
//}
///*
// * JNI方法
// * */
////JNIEXPORT jstring JNICALL
////Java_com_itfitness_httptest_MainActivity_stringFromJNI(
////        JNIEnv *env,
////        jobject jobj,jstring postUrl,jstring params) {
////    char * pu = Jstring2CStr(env,postUrl);
////    char * pm = Jstring2CStr(env,params);
////    char * result = http_post(pu,pm);
////    return (*env)->NewStringUTF(env,result);
////}
