#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <thread>
#include <functional>

#include "stl/bind.h"
#include "stl/function.h"
#include "stl/thread.h"
#include "stl/string.h"
#include "stl/queue.h"
#include "stl/construct.h"
#include "stl/list.h"
#include "stl/vector.h"
#include "stl/threadpool.h"
#include "project/connect.h"
#include "stl/brtree.h"



using namespace std;
using namespace stl;

class myclass {
public:
    int a = 10;

    myclass() {};
    myclass(int b) {
        a = b;
    }
};


void ABC1(int a, myclass b, int c, std::string d) {

    std::cout << "a " << a << " b " << b.a << " c " << c << " d " << d << std::endl;
}

void ABC2(int a, int b) {

    std::cout << a + b << std::endl;
    //return new int(100);
    //return 1001;
    //return a+b;
}

void ABC3(int a) {
    std::cout << "ABC3 " << a << std::endl;
}

int main() {












    // std::cout<<"300"<<std::endl;
    // auto temp = bind(ABC, 10, stl::placeholders::_1, 30, stl::placeholders::_2);
    // temp(myclass(10000), "321");

    //std::thread t(ABC, 10, myclass(10000), 30, "321");
    //std::bind(ABC, 10, myclass(10000), 30, "321");
    //tt





    // int lfd = socket(AF_INET, SOCK_STREAM, 0);
    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(10000);
    // addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    // bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
    // listen(lfd, 128);
    // stl::vector<int> socket_cfd(100);
    // int client_num = 0;
    // ThreadPool pool;
    // while (1) {

    //     struct sockaddr_in cliaddr;
    //     int clilen = sizeof(cliaddr);
    //     int cfd = accept(lfd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
    //     cout << "有一个客户端连接" << endl;
    //     socket_cfd.push_back(cfd);
    //     connectionstruct* conn_struct = new connectionstruct;
    //     conn_struct->cfd = cfd;
    //     conn_struct->client_id = socket_cfd;
    //     //stl::Thread thread(project::Connection_Http_Test, cfd);
    //     pool.submit(project::Connection_Http_Test, cfd);

    //     client_num++;
    //     if (client_num > 10) {
    //         cout << "连接数超过限制" << endl;
    //         break;
    //     }
    // }
    // close(lfd);
}