#pragma once
#include <sys/epoll.h>
#include <memory>
#include <vector>
#include "safesocket.h"
#include "channel.h"

class eventloop;

class poller : public std::enable_shared_from_this<poller> {
public:
    safesocket epollfd_;
    std::weak_ptr<eventloop> loop_;
    std::vector<std::reference_wrapper<channel>> channels_;
    std::vector<std::reference_wrapper<channel>> ve_;

    poller();
    void update(channel& ch);
    std::vector<std::reference_wrapper<channel>>& wait();

};