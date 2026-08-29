#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <sys/epoll.h>
#include "channel.h"

namespace muduo {
    class eventloop : public std::enable_shared_from_this<eventloop> {
    public:
        using task = std::function<void()>;
    public:
        int epollfd;
        std::vector<std::shared_ptr<channel>> channels_;
        std::queue<task> queue_;
    public:
        eventloop() {};
        void loop();
        void update(std::shared_ptr<channel> ch);


    };
}