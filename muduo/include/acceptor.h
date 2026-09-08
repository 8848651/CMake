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
#include "safesocket.h"


class acceptor {
public:
    using callback = std::function<void(safesocket)>;

public:
    safesocket sockfd_;
    callback readcallback_;
    std::shared_ptr<channel> connectchannel_;

    acceptor();
    void init(std::shared_ptr<eventloop> baseloop);
    void setcallback(callback readcallback);
    void newaccept();

    static safesocket getsocketfd() {
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(10000);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        safesocket socketfd;
        socketfd.createsafesocket(AF_INET, SOCK_STREAM, 0);
        socketfd.bindsafesocket((struct sockaddr*)&servaddr, sizeof(servaddr));
        socketfd.listensafesocket(128);
        return socketfd;
    }
};



