#pragma once
#include <functional>
#include <memory>
#include <iostream>
#include "eventloop.h"

class eventloop;

class channel : public std::enable_shared_from_this<channel>{
public:
    using callback=std::function<void()>;

public:
    int socketfd;
    std::weak_ptr<eventloop> loop_;
    callback readcallback;

    channel(int socketfd);
    channel(int socketfd,std::weak_ptr<eventloop> loop);
    void init(std::weak_ptr<eventloop> loop);
    void setreadcallback(callback readcallback_);
    void update();
    std::shared_ptr<channel> getshaared();



};