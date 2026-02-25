#include "poller.h"



poller::poller(eventloop& loop_) 
    :epollfd(epoll_create(1))
    ,channel_()
    ,loop(loop_) {};
    
void poller::update(std::shared_ptr<channel> channel_){
    channel_=channel_;
    struct epoll_event ev;
    ev.data.fd = channel_->socketfd;
    ev.data.ptr = channel_.get();      
    ev.events = EPOLLIN;           
    ::epoll_ctl(epollfd, EPOLL_CTL_ADD, channel_->socketfd, &ev);
}

std::vector<std::shared_ptr<channel>> poller::wait(){
    epoll_event evs[10];
    int infds = ::epoll_wait(epollfd, evs, 10, -1);
    std::vector<std::shared_ptr<channel>> ve;
    for(int i=0;i<infds;i++){
        ve.emplace_back(static_cast<channel*>(evs[i].data.ptr)->getshaared());
    }
    return ve;
}

