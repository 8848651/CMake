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
class poller;

class eventloop : public std::enable_shared_from_this<eventloop>{
public:
    using submittasktype=std::function<void()>;

public:
    int wakeupfd_;
    const pid_t threadid_;
    std::mutex mutex_;
    std::unique_ptr<poller> poller_;
    std::shared_ptr<channel> wakeupchannel_;
    std::vector<submittasktype> submittask;

    //注意这里初始化列表顺序是按照声明顺序
    eventloop();
    void init();
    void loop();
    void update(std::shared_ptr<channel> ch);
    void tosubmittask(submittasktype task);
    void readeventfd();
    void writeeventfd();
    void dopendingfunctors();

    static int createeventfd(){
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        return evtfd;
    }

};
