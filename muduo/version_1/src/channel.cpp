#include "version.h"
#include <iostream>

channel::channel(int fd_, bool isListen)
    : fd(fd_)
    , listenfd(isListen) {
    std::cout << "456" << std::endl;
}

bool channel::islisten() {
    return listenfd;
};