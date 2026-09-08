#pragma once
#include <functional>
#include <memory>
#include <iostream>
#include "safesocket.h"

class eventloop;

class channel : public std::enable_shared_from_this<channel> {
public:
    using callback = std::function<void()>;
    using writecallback = std::function<void(const void*, size_t)>;

public:
    safesocket socketfd_;
    std::weak_ptr<eventloop> loop_;
    callback readcallback_;

    channel(safesocket&& socketfd);
    channel(safesocket&& socketfd, std::weak_ptr<eventloop> loop);
    void init(std::weak_ptr<eventloop> loop);
    void setreadcallback(callback readcallback);
    void update();
    std::shared_ptr<channel> getshared();
    void asynwrite(const void* buf, size_t bytes) {
        socketfd_.write(buf, bytes);
    }
    void syncwrite(const void* buf, size_t bytes) {
        socketfd_.write(buf, bytes);
    }

    void asynread(void* buf, size_t bytes) {
        socketfd_.read(buf, bytes);
    }
    void syncread(void* buf, size_t bytes) {
        socketfd_.read(buf, bytes);
    }



};