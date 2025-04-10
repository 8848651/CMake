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

class myclass{
public:
    int x=1;
    void test(int a,int b,int c){
        std::cout<<x<<std::endl;
        std::cout<<a<<std::endl;
        std::cout<<b<<std::endl;
        std::cout<<c<<std::endl;
    }


};


int main() {


    myclass a;
    auto tt=stl::bind(&myclass::test,&a,10,stl::placeholders::_1,300);
    tt(30);



    return 0;
}
