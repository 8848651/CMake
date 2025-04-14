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
#include "channel.h"
#include "loop.h"

class version{
private:
    struct sockaddr_in servaddr;
public:
    int socketfd;
    int epollfd;
    version():epollfd(epoll_create(1)) {
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(8888);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        socketfd = socket(AF_INET, SOCK_STREAM, 0);
        bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
        listen(socketfd, 128);
    }
    ~version() {
        close(socketfd);
        close(epollfd);
    }
};


int test() {

    version ver;  
    Channel *servchannel = new Channel(ver.socketfd, true);
    
    struct epoll_event ev;
    ev.data.ptr = servchannel;     
    ev.events = EPOLLIN;          
    epoll_ctl(ver.epollfd, EPOLL_CTL_ADD, ver.socketfd, &ev); 
    Loop loop(ver.epollfd, ver.socketfd); 
    return 0;
}
