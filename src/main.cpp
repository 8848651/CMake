#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>


#include "stl/thread.h"
#include "stl/string.h"
#include "stl/queue.h"
#include "stl/construct.h"
#include "stl/list.h"
#include "stl/vector.h"
#include "stl/threadpool.h"
#include "project/connect.h"


using namespace std;
using namespace stl;
using namespace project;

class C {
public:
    int a;
    C() : a(1) {};
    C(int b) : a(b) {};
    void test() {
        a = a + 1;
    };

};




int main() {

    stl::string str("1");
    if (str == "1") {
        std::cout << "str is empty" << std::endl;
    }
    else {
        std::cout << "1111111111111111" << std::endl;
    }





    // int x = 100;
    // stl::Node<C> n1{ x };
    // stl::Node<C> n2{ x };
    // n1.next = &n2;

    // stl::NodeBase<C> nn = n1;
    // std::cout << "n1.next = " << n1.next << std::endl;
    // std::cout << "n2 = " << &n2 << std::endl;
    // std::cout << "nn.next=  " << nn.next << std::endl;
    // std::cout << "nn.ptr=  " << nn.ptr << std::endl;
    // std::cout << "ptr  " << &n1 << std::endl;
    // std::cout << "nn.data  " << nn->a << std::endl;
    //std::cout << "n1:data  " << n1.data << std::endl;
    //std::cout << "*nn  " << *nn << std::endl;


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
    //     ConnectionStruct* conn_struct = new ConnectionStruct;
    //     conn_struct->cfd = cfd;
    //     conn_struct->client_id = socket_cfd;
    //     //stl::Thread thread(tool::Connection_2, conn_struct);
    //     pool.submit(tool::Connection_2, conn_struct);

    //     client_num++;
    //     if (client_num > 2) {
    //         cout << "连接数超过限制" << endl;
    //         break;
    //     }
    // }
    // close(lfd);

}