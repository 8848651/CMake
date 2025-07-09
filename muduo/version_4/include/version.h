#pragma once


#include "tcpserver.h"


int test() {
    tcpserver server{ [](int socketfd) {
        pid_t t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        printf("t_cachedTid: %d\n", t_cachedTid);
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(socketfd, buf, sizeof(buf));
        if (len > 0) {
            printf("客户端: %s\n", buf);
        }
    } };
    server.start();

    return 0;
}
