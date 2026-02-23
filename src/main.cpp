#include <iostream>
#include <meta/tuple.h>
#include <meta/bind.h>
#include <meta/function.h>
#include "../muduo/version_5/include/tcpthread.h"




int main() {

    tcpthread thread{};
    if(thread.geteventloopptr() != nullptr){
        std::cout<<"123"<<std::endl;
    }
}
