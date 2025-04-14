#pragma once
#include "stl/function.h"

class Channel {
    public:
        using clallback = stl::function<void()>;
        int fd;
        bool listenfd;
        clallback clallback_t;
        Channel(int fd_, bool isListen = false) : fd(fd_), listenfd(isListen) {}
        bool islisten() const { return listenfd; }
};