#include "poller.h"



poller::poller() 
    :epollfd_(epoll_create(1))
    ,channels_() {};


void poller::init(std::weak_ptr<eventloop> loop){
    loop_=loop;
};
    
void poller::update(std::shared_ptr<channel> channel_){
    channels_.emplace_back(channel_);
    struct epoll_event ev;
    ev.data.fd = channel_->socketfd_;
    ev.data.ptr = channel_.get();      
    ev.events = EPOLLIN;           
    ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, channel_->socketfd_, &ev);
}

std::vector<std::shared_ptr<channel>> poller::wait(){
    epoll_event evs[10];
    int infds = ::epoll_wait(epollfd_, evs, 10, -1);
    std::vector<std::shared_ptr<channel>> ve;
    for(int i=0;i<infds;i++){
        ve.emplace_back(static_cast<channel*>(evs[i].data.ptr)->getshared());
    }
    return ve;
}

