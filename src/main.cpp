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
#include <sys/syscall.h>

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
#include "muduo/version_4/version.h"



using namespace std;
using namespace stl;

void* test_1(void*) {
    pid_t t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    printf("Thread ID: %d\n", t_cachedTid);
    return nullptr;
}



int main() {
    test();

    return 0;
}
