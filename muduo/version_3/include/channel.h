#pragma once
#include <functional>

class channel {
public:
    using clallback = std::function<void()>;
    int fd;
    bool listenfd;
    clallback clallback_t;
    channel(int fd_, bool isListen = false);
    bool islisten();
};