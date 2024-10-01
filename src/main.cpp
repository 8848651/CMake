#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>

#include "tool.h"
#include "stl/thread.h"
#include "stl/construct.h"
#include "stl/list.h"
#include "stl/vector.h"
#include "stl/threadpool.h"
#include "project/ConnectionStruct.h"


using namespace std;
using namespace tool;

class C {};




int main() {

    stl::is_same_temp<int>::type a;
    stl::is_same_temp<int*>::type b;
    stl::is_same_temp<C>::type c;

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;






    // int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(10000);
    // addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    // bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    // listen(lfd, 128);
    // int socket_cfd[100];
    // int client_num = 0;
    // while (1) {

    //     struct sockaddr_in cliaddr;
    //     int clilen = sizeof(cliaddr);
    //     int cfd = accept(lfd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
    //     cout << "有一个客户端连接" << endl;
    //     socket_cfd[client_num] = cfd;
    //     ConnectionStruct* conn_struct = new ConnectionStruct;
    //     conn_struct->cfd = cfd;
    //     conn_struct->client_id = socket_cfd;
    //     stl::Thread thread(tool::Connection_2, conn_struct);

    //     client_num++;
    //     if (client_num > 2) {
    //         cout << "连接数超过限制" << endl;
    //         break;
    //     }
    // }
    // close(lfd);

}