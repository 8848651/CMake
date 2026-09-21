#pragma once
#include <memory>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include "eventloop.h"
#include <mutex>
#include <condition_variable>

class tcpthread{
public:

    eventloop loop_;
    std::mutex mutex_;
    std::condition_variable cond_;
    // pthread_ 声明在最后：析构时最先析构的是它，但真正的 join 在析构函数体里做
    // （析构体先于成员析构执行），所以顺序是安全的。
    std::thread pthread_;

    tcpthread():loop_(),mutex_(),cond_(),pthread_([this](){
        loop_.loop();
    }){};

    ~tcpthread(){
        // 没有这个析构：std::thread 析构时如果还 joinable 就直接 std::terminate。
        // 而 loop_.loop() 里是 epoll_wait(-1)，光置退出标志不会自己醒，所以要先 quit() 唤醒它。
        if (!pthread_.joinable()) { return; }
        loop_.quit();
        pthread_.join();   // join 之后线程已结束，loop_ 才能安全析构
    };

    eventloop& getloop(){return loop_;};
};