#pragma once
#include <functional>

class eventloop;

class channel : public std::enable_shared_from_this<channel> {
public:
    using EventCallback = std::function<void()>;

public:
    channel(int socketfd, std::shared_ptr<eventloop> loop) :socketfd(socketfd), loop(loop) {};
    void setReadCallback(EventCallback cb) { readCallback_ = std::move(cb); }
    void updateForEpoll();

public:
    int socketfd;
    std::shared_ptr<eventloop> loop;
    EventCallback readCallback_;
};