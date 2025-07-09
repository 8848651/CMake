#pragma once
#include <sys/epoll.h>
#include "channel.h"

class eventloop {
public:
    eventloop() :epollfd(epoll_create(1)) {};
    void loop();
    void update(std::shared_ptr<channel> ch);

public:
    epoll_event evs[10];
    int epollfd;

};

void eventloop::loop() {
    while (true) {
        int infds = epoll_wait(epollfd, evs, 10, -1);
        for (int i = 0; i < infds; i++) {
            //channel* ch = (channel*)evs[i].data.ptr;
           // ch->readCallback_();
        }
    }
}

void eventloop::update(std::shared_ptr<channel> ch) {
    // epoll_event ev;
    // ev.data.ptr = ch;
    // ev.events = EPOLLIN | EPOLLET; // 边缘触发
    //epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->socketfd, &ev);
}