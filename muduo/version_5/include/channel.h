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
    eventloop& loop;
    callback readcallback;

    channel(int socketfd_,eventloop& loop_);
    void setreadcallback(callback readcallback_);
    void update();


};