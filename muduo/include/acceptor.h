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
    using callback = std::function<void(safesocket&&)>;

public:
    callback readcallback_;
    channel ch_;

    acceptor(callback callback_, eventloop& loop_) :ch_(getsocketfd(), std::forward<eventloop&>(loop_)) {
        ch_.setreadcallback(
            [this](channel& ch) {
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                safesocket clientfd = ch.getsafesocket().acceptsafesocket(clientaddr, len);
                clientfd.setnonblocking();
                readcallback_(std::move(clientfd));
            }
        );
        readcallback_ = callback_;
    }
    channel& getconnectfd() { return ch_; }


    void setcallback(callback readcallback) { readcallback_ = readcallback; }

    static safesocket getsocketfd() {
        struct sockaddr_in servaddr;
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(10000);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        safesocket socketfd;
        socketfd.createsocketfd(AF_INET, SOCK_STREAM, 0);
        socketfd.bindsafesocket((struct sockaddr*)&servaddr, sizeof(servaddr));
        socketfd.listensafesocket(128);
        return socketfd;
    }
};



