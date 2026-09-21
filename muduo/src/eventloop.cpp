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
    while (!quit_) {
        std::vector<std::reference_wrapper<channel>>& ve = poller_.wait();
        for (channel& vel : ve) {
            // 没设回调的 channel 不要直接调，否则 std::function 会抛 bad_function_call，
            // 异常会一路穿出 loop() 把整个事件循环打掉。
            if (vel.readcallback_) { vel.readcallback_(vel); }
        }
        dopendingfunctors();
    }
}

void eventloop::update(channel& ch) {
    poller_.update(ch);
}

void eventloop::remove(channel& ch) {
    poller_.remove(ch);
}

void eventloop::quit() {
    quit_ = true;
    // 必须唤醒：loop 线程此刻很可能正阻塞在 epoll_wait(-1) 上，只置位它看不到。
    writeeventfd();
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
