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

int add(ppt& a) { return 0; }

template<typename T>
T t1(T&& t) {
    return std::forward<T>(t);
};

template<typename T>
auto t2(T t) {};

template<typename T>
class tt;



int main() {

    ppt a1;

    using P = stl::typequeue<int&>;
    using Q = stl::typequeue<stl::placeholders>;

    using R = stl::typequeuereferenceassisted<P, Q>::type;
    //tt<R> _;


    //auto f1 = stl::bind(add, stl::placeholders::_1);
    // auto f2 = stl::bind(add, stl::placeholders::_1);
    // f2(a1);
    //int a=f1(100,10);
    //std::cout<<a<<std::endl;




    return 0;
}
