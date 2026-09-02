#pragma once
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <functional>
#include "channel.h"
#include "eventloop.h"


class acceptor {
public:
    using callback = std::function<void(int)>;

public:
    int sockfd_;
    callback readcallback_;
    std::shared_ptr<channel> connectchannel_;

    acceptor();
    void init(std::shared_ptr<eventloop> baseloop);
    void setcallback(callback readcallback);
    void newaccept();

    static int getsocketfd() {
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(10000);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        int socketfd = socket(AF_INET, SOCK_STREAM, 0);
        bind(socketfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
        listen(socketfd, 128);
        return socketfd;
    }

    //设置非阻塞
    static void setnonblocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }

};



