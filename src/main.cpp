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
#include <meta/typereplace.h>

#include <iostream>
#include <type_traits>
#include <typeinfo>

struct A {
    A() {}

    template<typename T>
    A(T&& t) {
        std::cout << "A(T&&) called\n";
        std::cout << "  T is: " << typeid(T).name() << "\n";
        std::cout << "  is T A? " << std::is_same<T, A>::value << "\n";
        std::cout << "  is T B? " << std::is_same<T, struct B>::value << "\n";
        std::cout << "  is T const A&? "
                  << std::is_same<T, const A&>::value << "\n";
        std::cout << "-------------------------\n";
    }
};

struct B : public A {
    B() {}
    B(const B& b) : A(static_cast<const A&>(b)) {}
};

template<typename T>
class tp;


int main() {

    char a='a';
    stl::tuple<char> data(a);
    stl::tuple<int> args(1);
    stl::parametertype<char,stl::typequeue<char>,stl::typequeue<int>> pool(data,args);
    auto b = pool.recell();
    


    


    return 0;
}
