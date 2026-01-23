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

int add(int a, int b) {
    return a + b;
}


int main() {

    auto f = stl::bind(add, 10, 20);
    std::cout << "Result: " << f() << std::endl;


    return 0;
}
