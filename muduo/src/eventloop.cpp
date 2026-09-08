#include "eventloop.h"
#include "channel.h"

eventloop::eventloop()
    :threadid_(::syscall(SYS_gettid))
    , poller_(std::make_unique<poller>()) {
    safesocket temp;
    temp.createsafeeventfd();
    wakeupchannel_ = std::make_shared<channel>(temp);
    wakeupchannel_->setreadcallback([this]() {readeventfd();});
}

void eventloop::init() {
    poller_->init(shared_from_this());
    wakeupchannel_->init(shared_from_this());
    wakeupchannel_->update();
}


void eventloop::loop() {
    while (true) {
        std::vector<std::shared_ptr<channel>> ve = poller_->wait();
        for (std::shared_ptr<channel> vel : ve) {
            vel->readcallback_();
        }
        dopendingfunctors();
    }
}

void eventloop::update(std::shared_ptr<channel> channel_) {
    poller_->update(channel_);
}


void eventloop::tosubmittask(submittasktype task) {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        submittask_.emplace_back(task);
    }
    writeeventfd();
}

void eventloop::readeventfd() {
    uint64_t one = 1;
    wakeupchannel_->syncread(&one, sizeof(one));
}

void eventloop::writeeventfd() {
    uint64_t one = 1;
    wakeupchannel_->syncwrite(&one, sizeof(one));
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



