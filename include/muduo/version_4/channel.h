#pragma once

#include <functional>


class channel {
    public:
        using EventCallback = std::function<void()>;

    public:
        channel(int socketfd):socketfd(socketfd){};
        void setReadCallback(EventCallback cb) { readCallback_ = std::move(cb); }

    public:
        int socketfd;
        EventCallback readCallback_;


};