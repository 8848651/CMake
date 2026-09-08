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

    channel(eventloop& loop);
    channel(safesocket&& socketfd, eventloop& loop);
    channel(const channel&) = delete;
    channel& operator=(const channel&) = delete;
    channel(channel&& other) noexcept;
    channel& operator=(safesocket&& other) noexcept;

    template<typename T>
    void setreadcallback(T readcallback) { readcallback_ = readcallback; }
    void setsafesocket(safesocket&& socketfd);
    safesocket& getsafesocket();
    void update();

    std::shared_ptr<channel> getshared();

    void asynwrite(const void* buf, size_t bytes);
    void syncwrite(const void* buf, size_t bytes);

    void asynread(void* buf, size_t bytes);
    void syncread(void* buf, size_t bytes);



};