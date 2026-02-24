#include "eventloop.h"



//注意这里初始化列表顺序是按照声明顺序
eventloop::eventloop()
    :_poller(*this)
    ,wakeupfd_(createeventfd())
    ,threadid_(::syscall(SYS_gettid))
    ,wakeupchannel_(std::make_shared<channel>(wakeupfd_,*this)){
        wakeupchannel_->setreadcallback([&](){readeventfd();});
    };

void eventloop::update(std::shared_ptr<channel> ch){
    _poller.update(ch);
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
}

void eventloop::writeeventfd(){
    uint64_t one = 1;
    ssize_t n = write(wakeupfd_, &one, sizeof one);
}
    


