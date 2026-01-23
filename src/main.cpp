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


int add1(int a, int b) {
    return a + b;
}

// stl::tuple<int, int> test() {
//     stl::tuple<int, int> t1(10, 10);
//     return t1;
// };


int main() {

    auto a = stl::typequantity<1, int, stl::typequeue<int>>::value;
    std::cout << a << std::endl;



    int x = 10;
    int y = 10;
    // auto f11 = stl::bind(add1, x,y);
    // std::cout <<"f11: "<< f11() << std::endl;
    //auto f12 = stl::bind(add1, x,stl::placeholders::_1);
    //std::cout <<"f12: "<< f12(y) << std::endl;
    // // auto f13 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // // std::cout <<"f13: "<< f13(x,y) << std::endl;


    // auto f21 = stl::bind(add1, 10,10);
    // std::cout <<"f21: "<< f21() << std::endl;
    // auto f22 = stl::bind(add1, 10,stl::placeholders::_1);
    // std::cout <<"f22: "<< f22(10) << std::endl;
    // auto f23 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // std::cout <<"f23: "<< f23(10,10) << std::endl;

    // auto f31 = stl::bind(add1, 10,x);
    // std::cout <<"f31: "<< f31() << std::endl;
    // // auto f32 = stl::bind(add1, 10,stl::placeholders::_1);
    // // std::cout <<"f32: "<< f32(x) << std::endl;
    // auto f33 = stl::bind(add1, stl::placeholders::_1,x);
    // std::cout <<"f33: "<< f33(10) << std::endl;
    // // auto f34 = stl::bind(add1, stl::placeholders::_1,stl::placeholders::_2);
    // // std::cout <<"f34: "<< f34(x,10) << std::endl;



    return 0;
}
