#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "tool.h"
#include "stl/thread.h"
#include "stl/vector.h"
#include "project/ConnectionStruct.h"


using namespace std;
using namespace tool;

void test() {
    cout << "abcde" << endl;
}



int main() {

    stl::Thread tt(test, 10, 20);



    // using abc = stl::AssistedQueue<10>::QueueData;
    // abc::print();

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
    //     pthread_t thread;
    //     struct sockaddr_in cliaddr;
    //     int clilen = sizeof(cliaddr);
    //     int cfd = accept(lfd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
    //     socket_cfd[client_num] = cfd;
    //     ConnectionStruct conn_struct;
    //     conn_struct.cfd = cfd;
    //     conn_struct.client_id = socket_cfd;
    //     pthread_create(&thread, NULL, tool::Connection, &conn_struct);
    //     pthread_detach(thread);
    //     client_num++;
    //     if (client_num > 2) {
    //         cout << "too many clients" << endl;
    //         break;
    //     }
    // }
    // close(lfd);

}