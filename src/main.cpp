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

class ppt {
public:
    int a = 0;
    ppt() {};
    ppt(const ppt& pt) :a(pt.a + 1) {
        std::cout << "这是第 " << a << " 次拷贝" << std::endl;
    }
};

int add(ppt a) { return 0; }

template<typename T>
auto tt(T t) {};


int main() {

    ppt a1;
    tt([=]() {return a1;});

    //auto f1 = stl::bind(add, stl::placeholders::_1);
    // auto f2 = stl::bind(add, a1);
    // f2();
    //int a=f1(100,10);
    //std::cout<<a<<std::endl;




    return 0;
}
