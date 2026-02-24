#pragma once
#include <sys/epoll.h>
#include "channel.h"
#include "eventloop.h"

class eventloop;
class channel;

class poller {
public:
    int epollfd;
    eventloop& loop;
    std::shared_ptr<channel> channel_;

    poller(eventloop& loop_);
    void update(std::shared_ptr<channel> ch);

};