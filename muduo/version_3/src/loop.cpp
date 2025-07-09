#include <cstring>
#include "channel.h"
#include "loop.h"


Loop::Loop(epoll_event(*_envs)[10], int _epollfd, int _socketfd) : epollfd(_epollfd), socketfd(_socketfd) {
    std::memcpy(evs, _envs, sizeof(evs));
}

void Loop::loop() {
    while (true) {
        int infds = epoll_wait(epollfd, evs, 10, -1);
        // 处理事件
        for (int ii = 0; ii < infds; ii++) {
            channel* ch = (channel*)evs[ii].data.ptr;
            if (ch->islisten() == true) {
                // 新客户端连接
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int clientfd = accept(socketfd, (struct sockaddr*)&clientaddr, &len);
                setnonblocking(clientfd);  // 设置非阻塞
                // 为新客户端注册读事件
                channel* clientchannel = new channel(clientfd);
                epoll_event ev;
                ev.data.ptr = clientchannel;
                ev.events = EPOLLIN | EPOLLET; // 边缘触发
                epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &ev);
            }
            else {
                // 这里可以处理客户端 fd 的读写事件
                // 例如：读取数据、处理断开连接等
            }
        }
    }
}

void Loop::setnonblocking(int fd) {
    // 获取当前文件描述符的状态标志
    int flags = fcntl(fd, F_GETFL, 0);
    // 设置新状态为非阻塞
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}