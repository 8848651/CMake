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
#include <iostream>
#include <functional>

template<typename T>
using MyFunction = stl::function<T>; 

// 函数指针
int func_int_ret(int x) { return x + 1; }
int func_void_param() { return 42; }
void func_void_ret(int x) { std::cout << "Value: " << x << std::endl; }

// 带有状态的lambda
auto lambda_int_ret = [](int x) -> int { return x * 2; };
auto lambda_void_param = []() -> int { return 123; };
auto lambda_void_ret = [](int x) { std::cout << "Lambda print: " << x << std::endl; };

// 用于bind测试的普通函数
int bind_target(int x) { return x + 10; }
int bind_no_param() { return 2025; }
void bind_void_ret(int x) { std::cout << "Bind void ret: " << x << std::endl; }

int main() {
    // 函数指针
    MyFunction<int(int)> f1 = func_int_ret;
    MyFunction<int()> f2 = func_void_param;
    MyFunction<void(int)> f3 = func_void_ret;

    std::cout << "f1(10) = " << f1(10) << std::endl;
    std::cout << "f2() = " << f2() << std::endl;
    f3(99);

    // lambda 表达式
    MyFunction<int(int)> l1 = lambda_int_ret;
    MyFunction<int()> l2 = lambda_void_param;
    MyFunction<void(int)> l3 = lambda_void_ret;

    std::cout << "l1(5) = " << l1(5) << std::endl;
    std::cout << "l2() = " << l2() << std::endl;
    l3(123);


    // std::bind
    MyFunction<int(int)> b1 = stl::bind(bind_target, stl::placeholders::_1);
    MyFunction<int()> b2 = stl::bind(bind_no_param);
    MyFunction<void(int)> b3 = stl::bind(bind_void_ret, stl::placeholders::_1);

    std::cout << "b1(7) = " << b1(7) << std::endl;
    std::cout << "b2() = " << b2() << std::endl;
    b3(456);

    return 0;
}
