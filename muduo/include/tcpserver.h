#pragma once
#include <functional>
#include <map>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

#include "eventloop.h"
#include "acceptor.h"
#include "channel.h"
#include <stdio.h>
#include "tcpthread.h"


template <typename T>
class tcpserver {
public:

    using callback = std::function<void(channel&)>;

public:
    callback messagecallback_;
    tcpthread jobthread_;
    eventloop loop_;                  
    std::map<int, channel> channels_; 
    acceptor acceptor_;

public:
    tcpserver() :messagecallback_(),jobthread_(), loop_(), channels_(), acceptor_([this](safesocket&& socketfd) {
        int fd = socketfd.getsocketfd();
        channel& ch = channels_.emplace(
            fd, channel{ std::move(socketfd), jobthread_.getloop() }).first->second;
        ch.setreadcallback(messagecallback_);
        ch.update();
        }, loop_) {
    };

    template <typename U = T>
    auto start() -> decltype(std::declval<U>().onmessage(std::declval<channel&>()),
        std::declval<U>().onconnect(std::declval<channel&>()), void()) {
        messagecallback_ = [this](channel& ch) {static_cast<U*>(this)->onmessage(ch);};
        loop_.loop();
    };

};