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
#include <meta/typereplace.h>

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



    return 0;
}
