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
#include "tcpconnection.h"


template <typename T>
class tcpserver {
public:
    using callback = std::function<void(std::shared_ptr<channel>)>;

public:
    callback messagecallback_;
    std::shared_ptr<eventloop> loop_;   
    std::vector<channel> channels_;
    acceptor acceptor_;
    std::thread jobthread_;

public:
    tcpserver() :loop_(std::make_shared<eventloop>()), acceptor_([this](safesocket&& socketfd) {
            channel ch{std::forward<safesocket>(socketfd)};
            ch->setreadcallback(messagecallback_);
            ch->update();
            channels_.emplace_back(std::forward<safesocket>(socketfd));
        }, loop_) {
    };

    template <typename U = T>
    auto start() -> decltype(std::declval<U>().onmessage(std::declval<std::shared_ptr<channel>>()),
        std::declval<U>().onconnect(std::declval<std::shared_ptr<channel>>()), void()) {
        messagecallback_ = [this](std::shared_ptr<channel> ch) {static_cast<U*>(this)->onmessage(ch);};
        loop_->loop();   
    };

};