#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>

// 假设 Channel 是你自己定义的类，封装了文件描述符和事件
class channel {
public:
    int fd;
    bool listenfd;

    channel(int fd_, bool isListen = false);
    bool islisten();
};

// 设置非阻塞
/**
 * @note
 * - fcntl 是一个通用的文件控制函数，用于操作文件描述符的属性。
 * - F_GETFL 获取文件描述符的当前状态标志。
 * - F_SETFL 设置文件描述符的状态标志。
 * - O_NONBLOCK 是一个标志，用于启用非阻塞模式。
 */
void setnonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int test() {

    struct sockaddr_in servaddr;
    //memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 128);

    int epollfd = epoll_create(1);
    channel* servchannel = new channel(listenfd, true);

    // 为 listenfd 注册读事件
    struct epoll_event ev;
    ev.data.ptr = servchannel;      // 指定自定义数据
    ev.events = EPOLLIN;           // 可读事件
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

    struct epoll_event evs[10];    // 存放 epoll_wait 返回的事件

    while (true) {
        int infds = epoll_wait(epollfd, evs, 10, -1);  // 等待事件发生
        // 处理事件
        for (int ii = 0; ii < infds; ii++) {
            channel* ch = (channel*)evs[ii].data.ptr;
            if (ch->islisten() == true) {
                // 新客户端连接
                struct sockaddr_in clientaddr;
                socklen_t len = sizeof(clientaddr);
                int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len);
                setnonblocking(clientfd);  // 设置非阻塞
                // 为新客户端注册读事件
                channel* clientchannel = new channel(clientfd);
                struct epoll_event ev;
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

    close(epollfd);
    close(listenfd);
    return 0;
}
