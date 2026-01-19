#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <vector>
#include <thread>
#include <functional>
#include <sys/syscall.h>
#include <meta/bind.h>

int add(int a, int b) { return a + b; }


int main() {

    auto f1 = stl::bind(add, 1, 2);
    int a=f1();
    std::cout<<a<<std::endl;





    return 0;
}
