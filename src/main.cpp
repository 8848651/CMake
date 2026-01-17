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

struct A {};
struct B {};
struct C {};
struct D {};
struct E {};

template<typename T>
struct print_type;

int&& test(int&& a) {
    return std::move(a);
}


int main() {

    auto abc = test(10);

    stl::tuple<int, int> temp{ 1,2 };
    auto a = stl::tuplefindelement<0>(temp);
    std::cout << a << std::endl;

    auto b = stl::tuplefindelement<1>(temp);
    std::cout << b << std::endl;

    int m = 10;
    int n = 20;
    stl::tuple<int&, int&> temp2{ m,n };
    auto c = stl::tuplefindelement<0>(temp2);
    std::cout << c << std::endl;

    auto d = stl::tuplefindelement<1>(temp2);
    std::cout << d << std::endl;

    // stl::tuple<int&&, int&&> temp3{ 100,200 };
    // int&& p = stl::tuplefindelement<0>(temp3);
    // std::cout << p << std::endl;

    // int&& q = stl::tuplefindelement<1>(temp3);
    // std::cout << q << std::endl;



    return 0;
}
