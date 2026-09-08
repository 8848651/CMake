#pragma once
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "poller.h"
#include "safesocket.h"
#include "channel.h"


class eventloop : public std::enable_shared_from_this<eventloop> {
public:
    using submittasktype = std::function<void()>;

public:
    const pid_t threadid_;
    std::mutex mutex_;
    poller poller_;
    channel ch_;
    std::vector<submittasktype> submittask_;

    eventloop();
    void loop();
    void update(channel& ch);
    void tosubmittask(submittasktype task);
    void dopendingfunctors();
    void readeventfd();
    void writeeventfd();

};
