#pragma once
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "poller.h"
#include "safesocket.h"
#include "channel.h"


// 说明：这里原本继承了 enable_shared_from_this，但 eventloop 从来不是由 shared_ptr 管理的
// （它是 tcpthread / tcpserver 的直接成员），一旦真去调 shared_from_this() 会抛 bad_weak_ptr，
// 所以去掉这一层继承更诚实。
class eventloop {
public:
    using submittasktype = std::function<void()>;

public:
    const pid_t threadid_;
    std::mutex mutex_;
    poller poller_;
    channel ch_;
    std::vector<submittasktype> submittask_;
    std::atomic<bool> quit_{ false };  

    eventloop();
    void loop();
    void update(channel& ch);
    void remove(channel& ch);
    void quit();                       
    void tosubmittask(submittasktype task);
    void dopendingfunctors();
    void readeventfd();
    void writeeventfd();

};
