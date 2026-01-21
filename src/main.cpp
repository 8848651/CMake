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



int main() {
    char a = 'a';

    stl::tuple<int,char,int> data(1,a,3);
    stl::tuple<int> args(1);
    stl::parametertype<char,stl::typequeue<int,char,int>,stl::typequeue<int>> type(data,args);
    auto tt=type.recell();








    return 0;
}
