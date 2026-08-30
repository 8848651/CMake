#include "tcpserver.h"
#include "channel.h"
#include <iostream>
#include <string>
#include <csignal>
#include <unistd.h>

// 用 CRTP 方式使用 muduo2：继承 tcpserver<自己的类>，实现 onconnect / onmessage
class echo_server : public muduo::tcpserver<echo_server> {
public:
    void onconnect(std::shared_ptr<muduo::channel> ch) {
        std::cout << "[onconnect] fd=" << ch->socketfd_ << " connected" << std::endl;
    }
    void onmessage(std::shared_ptr<muduo::channel> ch) {
        ch->read();   // 同步式写法：底层 yield 让 reactor 线程 read，read 完 resume 回这里
        if (ch->readsize_ > 0) {
            std::string msg(ch->buffer->data(), ch->readsize_);
            std::cout << "[onmessage] fd=" << ch->socketfd_ << " recv: " << msg;
            ::write(ch->socketfd_, ch->buffer->data(), ch->readsize_);
        }
    }
};

int main() {
    signal(SIGPIPE, SIG_IGN);   // 对端关闭后 write 不杀进程
    echo_server server;
    std::cout << "echo server listening on 10000, start() blocking..." << std::endl;
    server.start();   // 阻塞运行 reactor
    return 0;
}
