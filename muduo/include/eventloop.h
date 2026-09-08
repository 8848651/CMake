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


class channel;
class poller;

class eventloop : public std::enable_shared_from_this<eventloop>{
public:
    using submittasktype=std::function<void()>;

public:
    const pid_t threadid_;
    std::mutex mutex_;
    std::unique_ptr<poller> poller_;
    std::shared_ptr<channel> wakeupchannel_;
    std::vector<submittasktype> submittask_;

    eventloop();
    void init();
    void loop();
    void update(std::shared_ptr<channel> ch);
    void tosubmittask(submittasktype task);
    void readeventfd();
    void writeeventfd();
    void dopendingfunctors();

};
