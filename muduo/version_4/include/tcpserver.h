#pragma once
#include <memory>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

#include "eventloop.h"
#include "acceptor.h"
#include "channel.h"
#include <stdio.h>


// 1: baseloop
// 2: eventloop
class tcpserver {
public:
    using readblack = std::function<void(int)>;
public:
    //主做消息转发
    eventloop baseloop;
    //主做消息处理
    eventloop eventloop_;
    acceptor acceptor_;
    //tcpserver使用时的回调
    readblack readblack_;
    std::thread t;

public:
    void start();
    void acceptserver(int);
    tcpserver(readblack readblack_) :t([&]() {eventloop_.loop();}), readblack_(readblack_)
        , acceptor_(std::shared_ptr<eventloop>(&baseloop)) {
        acceptor_.setReadCallback([&](int clientfd) {acceptserver(clientfd);});
    };



};

void tcpserver::start() {
    pid_t t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
    printf("t_cachedTid: %d\n", t_cachedTid);
    acceptor_.connect_channel->updateForEpoll();
    baseloop.loop();
}

void tcpserver::acceptserver(int socketfd) {
    std::shared_ptr<channel> businesschannel = std::make_shared<channel>(socketfd, std::shared_ptr<eventloop>(&eventloop_));
    businesschannel->setReadCallback([&]() {readblack_(socketfd);});
    businesschannel->updateForEpoll();
}