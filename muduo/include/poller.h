#pragma once
#include <sys/epoll.h>
#include <memory>
#include <vector>
#include "safesocket.h"
#include "channel.h"

// poller 只做两件事：把 channel 注册/注销到 epoll，以及返回本轮就绪的 channel。
class poller {
public:
    safesocket epollfd_;
    std::vector<std::reference_wrapper<channel>> ve_;

    poller();
    void update(channel& ch);   
    void remove(channel& ch);  
    std::vector<std::reference_wrapper<channel>>& wait();

};