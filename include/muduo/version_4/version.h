#pragma once


#include "channel.h"
#include "eventloop.h"
#include "tcpserver.h"

int test() {
    tcpserver server{[](int socketfd){}};
    server.start();

    return 0;
}
