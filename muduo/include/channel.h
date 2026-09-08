#pragma once
#include <functional>
#include <memory>
#include <iostream>
#include "safesocket.h"

class eventloop;

class channel : public std::enable_shared_from_this<channel> {
public:
    using callback = std::function<void(channel&)>;

public:
    safesocket socketfd_;
    eventloop& loop_;
    callback readcallback_;

    channel(eventloop& loop) :loop_(std::forward<eventloop&>(loop)) {}
    channel(safesocket&& socketfd, eventloop& loop) :loop_(std::forward<eventloop&>(loop)) {}
    channel(const channel&) = delete;
    channel& operator=(const channel&) = delete;
    channel(channel&& other) noexcept :
        socketfd_(std::forward<safesocket>(other.socketfd_))
        , readcallback_(other.readcallback_)
        , loop_(other.loop_) {
    }
    channel& operator=(safesocket&& other) noexcept {}

    template<typename T>
    void setreadcallback(T readcallback) { readcallback_ = readcallback; }
    void setsafesocket(safesocket&& socketfd) { socketfd_ = std::move(socketfd); }
    safesocket& getsafesocket() { return socketfd_; }
    void update() { loop_.update(*this); }

    std::shared_ptr<channel> getshared() { return shared_from_this(); }

    void asynwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }
    void syncwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }

    void asynread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }
    void syncread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }



};