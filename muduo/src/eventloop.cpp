#include "eventloop.h"

eventloop::eventloop() :threadid_(::syscall(SYS_gettid))
, poller_()
, ch_(*this) {
    safesocket temp;
    temp.createeventfd();
    ch_.setsafesocket(std::move(temp));
    ch_.setreadcallback([this](channel& ch) {readeventfd();});
    ch_.update();
}

void eventloop::loop() {
    while (true) {
        std::vector<std::reference_wrapper<channel>>& ve = poller_.wait();
        for (channel& vel : ve) {
            vel.readcallback_(vel);
        }
        dopendingfunctors();
    }
}

void eventloop::update(channel& ch) {
    poller_.update(ch);
}

void eventloop::tosubmittask(submittasktype task) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        submittask_.emplace_back(task);
    }
    writeeventfd();
}

void eventloop::dopendingfunctors() {
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

void eventloop::readeventfd() {
    uint64_t one = 1;
    ch_.syncread(&one, sizeof(one));
}

void eventloop::writeeventfd() {
    uint64_t one = 1;
    ch_.syncwrite(&one, sizeof(one));
}
