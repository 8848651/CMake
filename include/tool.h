#pragma once
#include <string.h>
#include <stdio.h>

#include "project/ConnectionStruct.h"

namespace tool {

    //这是将字符ip转为sockaddr_in->sin_addr->s_addr(int32_t)的函数
    //即 192.168.1.1 ->  192存第一个字节，168存第二个字节，1存第三个字节，1存第四个字节
    //inet_pton(AF_INET, "192.168.136.129", &addr.sin_addr.s_addr)
    int StrToIn_Addr(const char* ip_address) {
        int tmp = 0;
        char* endptr = (char*)&tmp;
        char ip_copy[16];
        strcpy(ip_copy, ip_address);

        char* token = strtok(ip_copy, ".");
        while (token != NULL) {
            int num = atoi(token);
            *endptr = (char)num;
            endptr++;
            token = strtok(NULL, ".");
        }
        return tmp;
    }

    //连接后处理方法
    void* Connection_1(void* arg) {
        ConnectionStruct* conn_struct = (ConnectionStruct*)arg;
        int cfd = conn_struct->cfd;
        void* args = conn_struct->client_id;
        while (1) {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int len = read(cfd, buf, sizeof(buf));
            if (len > 0) {
                printf("data from client: %s\n", buf);
                write(cfd, buf, len);
            }
            else {
                break;
            }
        }
        close(cfd);
    }

    //连接后处理方法
    void Connection_2(ConnectionStruct* conn_struct) {
        int cfd = conn_struct->cfd;
        void* args = conn_struct->client_id;
        while (1) {
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int len = read(cfd, buf, sizeof(buf));
            if (len > 0) {
                printf("data from client: %s\n", buf);
                write(cfd, buf, len);
            }
            else {
                break;
            }
        }
        close(cfd);
    }

}