#pragma once
#include "channel.h"
#include <stdio.h>
#include <unistd.h>
//#include <cstring>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

class Loop {
public:
    using epoll_event = struct epoll_event;

public:
    epoll_event evs[10];
    int epollfd;
    int socketfd;

    Loop(int _epollfd, int _socketfd);
    Loop(epoll_event(*_envs)[10], int _epollfd, int _socketfd);

    void loop();

    //设置非阻塞
    void setnonblocking(int fd);

};