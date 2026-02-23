#pragma once
#include <sys/epoll.h>
#include "channel.h"

class poller {
public:
    poller(eventloop& _loop) 
        :epollfd(epoll_create(1))
        ,_channel(nullptr)
        ,loop(_loop) {};
    
    void update(std::shared_ptr<channel> ch){
        _channel=ch;
        struct epoll_event ev;
        ev.data.ptr = ch.get();      
        ev.events = EPOLLIN;           
        epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->socketfd, &ev);
    }

public:
    int epollfd;
    eventloop& loop;
    std::shared_ptr<channel> _channel;

};