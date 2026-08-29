#pragma once
#include <vector>
#include <memory>
#include <sys/epoll.h>
#include "channel.h"

namespace muduo {
    class eventloop : public std::enable_shared_from_this<eventloop> {
    public:
        using submittasktype = std::function<void()>;
    public:
        int epollfd;
        std::vector<std::shared_ptr<channel>> channels_;
        std::vector<submittasktype> submittask;
    public:
        eventloop() {};
        void loop();
        void update(std::shared_ptr<channel> ch);


    };
}