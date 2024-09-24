#pragma once
#include <string.h>
#include <stdio.h>

namespace Utilities {

    //这是将字符ip转为sockaddr_in->sin_addr->s_addr(int32_t)的函数
    //即 192.168.1.1 ->  192存第一个字节，168存第二个字节，1存第三个字节，1存第四个字节
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

}