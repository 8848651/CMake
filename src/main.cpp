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
#include <meta/tuple.h>
#include <meta/bind.h>

#include <iostream>
#include <type_traits>
#include <typeinfo>

//int 
//const int
//int&
//const int&
//int&&
//const int&&

//int&
//const int&
//int&&
//const int&&
//  -> const int&


int add1(int a, int b) {
    return a + b;
}

int add2(int& a, int& b) {
    a = 100;
    return a + b;
}

int add_rvalue(int&& a, int&& b) {
    std::cout << "add_rvalue called\n";
    return a + b;
}

int add_mix(int&& a, int& b) {
    a = 50;
    return a + b;
}


int main() {


    int x = 10;
    int y = 20;

    // 纯右值调用
    auto f1 = stl::bind(add_rvalue, stl::placeholders::_1, stl::placeholders::_2);
    std::cout << "f1: " << f1(10, 20) << std::endl;

    // // bind 阶段给右值
    auto f2 = stl::bind(add_rvalue, 10, 20);
    std::cout << "f2: " << f2() << std::endl;

    // 混合：右值 + 左值
    auto f3 = stl::bind(add_mix, stl::placeholders::_1, y);
    std::cout << "f3: " << f3(10) << std::endl;
    std::cout << "y: " << y << std::endl;


    // int x = 10;
    // int y = 10;
    // auto f11 = stl::bind(add1, x,y);
    // std::cout <<"f11: "<< f11() << std::endl;
    // auto f12 = stl::bind(add1, x,stl::placeholders::_1);
    // std::cout <<"f12: "<< f12(y) << std::endl;
    // auto f13 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // std::cout <<"f13: "<< f13(x,y) << std::endl;


    // auto f21 = stl::bind(add1, 10,10);
    // std::cout <<"f21: "<< f21() << std::endl;
    // auto f22 = stl::bind(add1, 10,stl::placeholders::_1);
    // std::cout <<"f22: "<< f22(10) << std::endl;
    // auto f23 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // std::cout <<"f23: "<< f23(10,10) << std::endl;

    // auto f31 = stl::bind(add1, 10,x);
    // std::cout <<"f31: "<< f31() << std::endl;
    // auto f32 = stl::bind(add1, 10,stl::placeholders::_1);
    // std::cout <<"f32: "<< f32(x) << std::endl;
    // auto f33 = stl::bind(add1, stl::placeholders::_1,x);
    // std::cout <<"f33: "<< f33(10) << std::endl;
    // auto f34 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // std::cout <<"f34: "<< f34(x,10) << std::endl;

    // auto f41 = stl::bind(add2, x,y);
    // std::cout << "f41: " << f41() << std::endl;
    // std::cout << "x: " << x << std::endl;
    // auto f42 = stl::bind(add2, x, stl::placeholders::_1);
    // std::cout << "f42: " << f42(y) << std::endl;
    // std::cout << "x: " << x << std::endl;
    // auto f43 = stl::bind(add2, stl::placeholders::_1, stl::placeholders::_2);
    // std::cout << "f43: " << f43(x, y) << std::endl;
    // std::cout << "x: " << x << std::endl;



    return 0;
}
