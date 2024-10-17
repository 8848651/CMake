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


    stl::BrTree<int, int> tree;
    tree.insert(5, 0);
    tree.insert(2, 0);
    tree.insert(4, 0);
    tree.insert(3, 0);
    //tree.insert(1, 0);




    //tree.get_max_point_2(tree.root_ptr);

    // std::cout << "max: " << (*tree.get_max_point_1(tree.root_ptr))->first << std::endl;
    // std::cout << "max: " << (*tree.get_max_point_2(tree.root_ptr))->first << std::endl;
    // std::cout << "max: " << (*tree.get_max_point(tree.root_ptr))->first << std::endl;
    // std::cout << "min: " << (*tree.get_min_point(tree.root_ptr))->first << std::endl;

    tree.remove(2);
    std::cout << tree.node_size(tree.root_ptr) << std::endl;
    tree.traverse_test();



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