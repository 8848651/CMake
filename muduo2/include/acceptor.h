#pragma once
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include <functional>
#include "channel.h"
#include <cstring>
#include <iostream>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "eventloop.h"

namespace muduo {
    class acceptor {
    public:
        using callback = std::function<void(std::shared_ptr<channel>)>;
    public:
        callback connectcallback_;
        std::shared_ptr<channel> ch_;
        std::shared_ptr<eventloop>& loop_;
    public:
        acceptor(callback connectcallback, std::shared_ptr<eventloop>& loop) :connectcallback_(connectcallback), loop_(loop),
            ch_(std::make_shared<channel>(
                getsocketfd(),
                [this](std::shared_ptr<channel> ch) {
                    struct sockaddr_in clientaddr;
                    socklen_t len = sizeof(clientaddr);
                    int clientfd = accept(ch->socketfd_, (struct sockaddr*)&clientaddr, &len);
                    setnonblocking(clientfd);
                    auto _ch = std::make_shared<channel>(clientfd, loop_);
                    connectcallback_(_ch);
                }
            )
            ){
            ch_->update();
        };

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
}