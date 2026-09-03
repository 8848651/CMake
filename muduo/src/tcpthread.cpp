#include "tcpthread.h"




tcpthread::tcpthread()
    :eventloop_()
    ,pthread_([&](){start();}){
    pthread_.detach(); 
}

void tcpthread::start(){
    std::unique_lock<std::mutex> lock(mutex_);
    eventloop_=std::make_shared<eventloop>();
    eventloop_->init();
    cond_.notify_one();
}

std::shared_ptr<eventloop> tcpthread::geteventloopptr(){
    std::unique_lock<std::mutex> lock(mutex_);
    while ( eventloop_ == nullptr ){
        cond_.wait(lock);
    }
    return eventloop_;
}

