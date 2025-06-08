#pragma once
#include "channel.h"
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "acceptor.h"

class eventloop {
    public:
        eventloop():epollfd(epoll_create(1)){};
        void loop();
        void update(channel* ch);

    public:
        epoll_event evs[10];
        int epollfd;

};

void eventloop::loop(){
    while (true) {
        int infds = epoll_wait(epollfd, evs, 10, -1); 
        for (int i = 0; i < infds; i++) {
            channel *ch = (channel *)evs[i].data.ptr;
            ch->readCallback_();
        }
    }
}

void eventloop::update(channel* ch){
    epoll_event ev;
    ev.data.ptr = ch;
    ev.events = EPOLLIN | EPOLLET; // 边缘触发
    epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->socketfd, &ev);
}