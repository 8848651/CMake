#pragma once
#include <sys/epoll.h>
#include "channel.h"
#include "eventloop.h"

class eventloop;
class channel;

class poller : public std::enable_shared_from_this<poller> {
public:
    int epollfd;
    std::weak_ptr<eventloop> loop_;
    std::vector<std::shared_ptr<channel>> channels_;

    poller();
    void init(std::weak_ptr<eventloop> loop);
    void update(std::shared_ptr<channel> ch);
    std::vector<std::shared_ptr<channel>> wait();

};