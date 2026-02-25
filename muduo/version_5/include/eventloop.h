#pragma once
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "channel.h"
#include "poller.h"

class channel;

class eventloop{
public:
    using submittasktype=std::function<void()>;

public:
    poller _poller;
    int wakeupfd_;
    const pid_t threadid_;
    std::mutex mutex_;
    std::shared_ptr<channel> wakeupchannel_;
    std::vector<submittasktype> submittask;

    //注意这里初始化列表顺序是按照声明顺序
    eventloop();

    void loop();

    void update(std::shared_ptr<channel> ch);
    void tosubmittask(submittasktype task);
    void dopendingfunctors();

    static int createeventfd(){
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        return evtfd;
    }

    static void readeventfd(int eventfd){
        uint64_t one = 1;
        ssize_t n = read(eventfd, &one, sizeof one);
    };
    
    static void writeeventfd(int eventfd){
        uint64_t one = 1;
        ssize_t n = write(eventfd, &one, sizeof one);
    }

};
