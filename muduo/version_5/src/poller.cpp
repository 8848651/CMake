#include "poller.h"



poller::poller(eventloop& loop_) 
    :epollfd(epoll_create(1))
    ,channel_()
    ,loop(loop_) {};
    
void poller::update(std::shared_ptr<channel> ch){
    channel_=ch;
    struct epoll_event ev;
    ev.data.ptr = ch.get();      
    ev.events = EPOLLIN;           
    epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->socketfd, &ev);
}

