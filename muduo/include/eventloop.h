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

class eventloop : public std::enable_shared_from_this<eventloop> {
public:
    using submittasktype = std::function<void()>;

public:
    const pid_t threadid_;
    std::mutex mutex_;
    poller poller_;
    channel ch_;
    std::vector<submittasktype> submittask_;

    eventloop() :threadid_(::syscall(SYS_gettid))
        , poller_()
        , ch_(*this) {
        safesocket temp;
        temp.createeventfd();
        ch_.setsafesocket(std::move(temp));
        ch_.setreadcallback([this](channel& ch) {readeventfd();});
        ch_.update();
    };

    void loop() {
        while (true) {
            std::vector<std::reference_wrapper<channel>>& ve = poller_.wait();
            for (channel& vel : ve) {
                vel.readcallback_(vel);
            }
            dopendingfunctors();
        }
    }
    void update(channel& ch) {
        poller_.update(ch);
    }

    void tosubmittask(submittasktype task) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            submittask_.emplace_back(task);
        }
        writeeventfd();
    }

    void dopendingfunctors() {
        if (submittask_.size() == 0) { return; };
        std::vector<submittasktype> submittasks;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            submittasks.swap(submittask_);
        }
        for (submittasktype task : submittasks) {
            task();
        }
    }

    void readeventfd() {
        uint64_t one = 1;
        ch_.syncread(&one, sizeof(one));
    }

    void writeeventfd() {
        uint64_t one = 1;
        ch_.syncwrite(&one, sizeof(one));
    }

};
