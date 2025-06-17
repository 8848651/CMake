#pragma once
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
//#include <tcl8.6/tcl-private/compat/string.h>
#include "channel.h"

int get_socket_fd(){
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(10000);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(socketfd, 128);
    return socketfd;
}

//设置非阻塞
void setnonblocking(int fd) {
    // 获取当前文件描述符的状态标志
    int flags = fcntl(fd, F_GETFL, 0);
    // 设置新状态为非阻塞
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

class acceptor {
    public:
        using EventCallback = std::function<void(int)>;

    public:
        EventCallback Callback_;
        channel connect_channel;

    public:
        acceptor():connect_channel(get_socket_fd()){
            connect_channel.setReadCallback([&](){acceptor::accepter();});
        };
        void accepter();
        void setReadCallback(EventCallback cb) { Callback_ = std::move(cb); }

};

void acceptor::accepter(){
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(connect_channel.socketfd, (struct sockaddr*)&clientaddr, &len);
    setnonblocking(clientfd);
    Callback_(clientfd);  
}


