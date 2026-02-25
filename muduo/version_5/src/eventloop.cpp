#include "eventloop.h"



//注意这里初始化列表顺序是按照声明顺序
eventloop::eventloop()
    :_poller(*this)
    ,wakeupfd_(createeventfd())
    ,threadid_(::syscall(SYS_gettid))
    ,wakeupchannel_(std::make_shared<channel>(wakeupfd_,*this)){
        wakeupchannel_->setreadcallback([](int eventfd){readeventfd(eventfd);});
        wakeupchannel_->update();
    };


void eventloop::loop(){
    while(true){
        std::vector<std::shared_ptr<channel>> ve = _poller.wait();
        for(std::shared_ptr<channel> vel : ve){
            vel->executecallback();
        }
    }
}

void eventloop::update(std::shared_ptr<channel> channel_){
    _poller.update(channel_);
}


void eventloop::tosubmittask(submittasktype task){
    {
        std::unique_lock<std::mutex> lock(mutex_);
        submittask.emplace_back(task);
    }
    writeeventfd(wakeupfd_);
};


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
    


