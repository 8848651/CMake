#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include "channel.h"

namespace muduo {
    class eventloop : public std::enable_shared_from_this<eventloop> {
    public:
        using task = std::function<void()>;
    public:
        int epollfd_;
        std::vector<std::shared_ptr<channel>> channels_;
        std::queue<task> queue_;
    public:
        eventloop() :epollfd_(epoll_create(1)) {};
        void loop();
        void update(std::shared_ptr<channel> ch);
    };
}