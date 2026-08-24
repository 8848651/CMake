#pragma once
#include <memory>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include "eventloop.h"
#include <mutex>
#include <condition_variable>


class tcpthread {
public:
    //这里不能使用引用，因为eventloop需要在线程中延迟创建，不是tcpthread创建时创建
    std::shared_ptr<eventloop> eventloop_;
    std::thread pthread;
    std::mutex mutex_;
    std::condition_variable cond_;

    tcpthread();
    void start();
    std::shared_ptr<eventloop> geteventloopptr();


};