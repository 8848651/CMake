#include "channel.h"
#include <iostream>

channel::channel(int fd_, bool isListen)
    : fd(fd_)
    , listenfd(isListen) {
    std::cout << "123" << std::endl;
}

bool channel::islisten() {
    return listenfd;
}