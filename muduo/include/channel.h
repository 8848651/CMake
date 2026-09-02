#pragma once
#include <functional>
#include <memory>
#include <iostream>

class eventloop;

class channel : public std::enable_shared_from_this<channel>{
public:
    using callback=std::function<void()>;

public:
    int socketfd_;
    std::weak_ptr<eventloop> loop_;
    callback readcallback_;

    channel(int socketfd);
    channel(int socketfd,std::weak_ptr<eventloop> loop);
    void init(std::weak_ptr<eventloop> loop);
    void setreadcallback(callback readcallback);
    void update();
    std::shared_ptr<channel> getshared();



};