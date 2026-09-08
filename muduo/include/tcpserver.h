#pragma once
#include <memory>
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
    using callback = std::function<void(std::shared_ptr<channel>)>;

public:
    callback messagecallback_;
    eventloop loop_;
    std::vector<channel> channels_;
    acceptor acceptor_;
    std::thread jobthread_;

public:
    tcpserver() :loop_(), channels_(), acceptor_([this](safesocket&& socketfd) {
        channel ch{ std::forward<safesocket>(socketfd),loop_ };
        ch->setreadcallback(messagecallback_);
        ch->update();
        channels_.emplace_back(std::move(ch));
        }, loop_) {
    };

    template <typename U = T>
    auto start() -> decltype(std::declval<U>().onmessage(std::declval<std::shared_ptr<channel>>()),
        std::declval<U>().onconnect(std::declval<std::shared_ptr<channel>>()), void()) {
        messagecallback_ = [this](std::shared_ptr<channel> ch) {static_cast<U*>(this)->onmessage(ch);};
        loop_->loop();
    };

};