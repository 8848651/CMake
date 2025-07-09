#pragma once
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
#include "channel.h"
#include "loop.h"


int test() {

    struct sockaddr_in servaddr;
    //std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(socketfd, 128);

    int epollfd = epoll_create(1);
    channel* servchannel = new channel(socketfd, true);

    // 为 socketfd 注册读事件
    struct epoll_event ev;
    ev.data.ptr = servchannel;
    ev.events = EPOLLIN;
    // 将监听套接字 socketfd 添加到 epoll 实例中，监听可读事件
    epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfd, &ev);
    Loop loop(epollfd, socketfd);

    close(epollfd);
    close(socketfd);
    return 0;
}
