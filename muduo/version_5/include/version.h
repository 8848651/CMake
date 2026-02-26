#pragma once
#include "channel.h"


#include "tcpserver.h"


int test() {
    tcpserver server{ [](channel ch) {
        pid_t t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        printf("t_cachedTid: %d\n", t_cachedTid);
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        int len = read(ch.socketfd, buf, sizeof(buf));
        if (len > 0) {
            printf("客户端: %s\n", buf);
        }
    } };


    return 0;
}
