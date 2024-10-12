#pragma once
#include <string.h>
#include <stdio.h>
#include "stl/vector.h"
#include <unistd.h>

namespace project {

    class connectionstruct {
    public:
        int cfd;
        //TODO: 以后将其换为vector不必担心长度问题
        stl::vector<int> client_id;
        connectionstruct() {};
        connectionstruct(int cfd, void* args) {
            //this->cfd = cfd;
            //this->client_id = (int(*)[10])args;
        };
    };

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
        connectionstruct* conn_struct = (connectionstruct*)arg;
        int cfd = conn_struct->cfd;
        //void* args = conn_struct->client_id;
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
    void Connection_2(connectionstruct* conn_struct) {
        int cfd = conn_struct->cfd;
        //void* args = conn_struct->client_id;
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
    void Connection_3(int cfd, stl::vector<int>* args) {
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
    void Connection_Http_Test(int cfd) {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        int len = read(cfd, buffer, sizeof(buffer) - 1);
        std::cout << "len:" << len << std::endl;
        stl::string str(buffer);
        std::cout << "len:" << str.length() << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << buffer << std::endl;
        std::cout << "-----------------------------------" << std::endl;

        for (int i = 0; i < len; i++) {
            if (buffer[i] == '\r') {
                buffer[i] = '-';
            }
            if (buffer[i] == '\n') {
                buffer[i] = '+';
            }
        }

        std::cout << buffer << std::endl;


        /*
len:323
len:323
-----------------------------------
GET /select?id=10 HTTP/1.1
Host: 192.168.136.129:10000
Connection: keep-alive
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/129.0.0.0 Safari/537.36 Edg/129.0.0.0
Accept: *\/*
Origin: null
Accept - Encoding : gzip, deflate
Accept - Language : zh - CN, zh;q = 0.9, en;q = 0.8


---------------------------------- -
GET / select ? id = 10 HTTP / 1.1 - +Host : 192.168.136.129 : 10000 - +Connection : keep - alive - +User - Agent : Mozilla / 5.0 (Windows NT 10.0; Win64; x64) AppleWebKit / 537.36 (KHTML, like Gecko) Chrome / 129.0.0.0 Safari / 537.36 Edg / 129.0.0.0 - +Accept : *\/*-+Origin: null-+Accept-Encoding: gzip, deflate-+Accept-Language: zh-CN,zh;q=0.9,en;q=0.8-+-+
         */


        const char* response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, World!";
        write(cfd, response, strlen(response));
        close(cfd);
    }

}