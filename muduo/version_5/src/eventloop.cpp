#include "eventloop.h"



//注意这里初始化列表顺序是按照声明顺序
eventloop::eventloop()
    :poller_(*this)
    ,wakeupfd_(createeventfd())
    ,threadid_(::syscall(SYS_gettid))
    ,wakeupchannel_(std::make_shared<channel>(wakeupfd_,*this)){
        wakeupchannel_->setreadcallback([&](){readeventfd();});
        wakeupchannel_->update();
    };


void eventloop::loop(){
    while(true){
        std::vector<std::shared_ptr<channel>> ve = poller_.wait();
        for(std::shared_ptr<channel> vel : ve){
            vel->readcallback();
        }
    }
}

void eventloop::update(std::shared_ptr<channel> channel_){
    poller_.update(channel_);
}


void eventloop::tosubmittask(submittasktype task){
    {
        std::unique_lock<std::mutex> lock(mutex_);
        submittask.emplace_back(task);
    }
    writeeventfd();
};

void eventloop::readeventfd(){
    uint64_t one = 1;
    ssize_t n = read(wakeupfd_, &one, sizeof one);
};
    
void eventloop::writeeventfd(){
    uint64_t one = 1;
    ssize_t n = write(wakeupfd_, &one, sizeof one);
}


void eventloop::dopendingfunctors(){
    std::vector<submittasktype> submittasks;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        submittasks.swap(submittask);
    }
    for(submittasktype task : submittask){
        task();
    }
}
    


