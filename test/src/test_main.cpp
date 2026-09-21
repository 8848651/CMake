// 针对 muduo/ 这一层的回归测试（不算集成测试，暂时不起真实连接）。
// 覆盖本次修复的点：
//   1. safesocket::createepollfd / createeventfd 真的把 fd 存进了成员
//      （原来是 `int fd_ = ...` 声明了局部变量遮蔽成员，成员一直是 -1，epoll 根本没建起来）
//   2. channel 用 index_ 区分 ADD / MOD，重复 update() 不再因 EEXIST 抛异常
//   3. channel 析构自动从 epoll 注销；被移动过的 channel 不会重复注销
//   4. channel(safesocket&&, eventloop&) 真的接管了 fd（原来形参根本没被使用）
//   5. tcpserver 的监听 channel 会被注册进 epoll；tcpthread 能正常 quit + join 而不 terminate
#include "eventloop.h"
#include "channel.h"
#include "safesocket.h"
#include "tcpserver.h"
#include <cstdio>
#include <thread>

static int g_failed = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (cond) {                                                    \
            printf("[ ok ] %s\n", #cond);                              \
        } else {                                                       \
            printf("[FAIL] %s  (%s:%d)\n", #cond, __FILE__, __LINE__); \
            ++g_failed;                                                \
        }                                                              \
    } while (0)

class testserver : public tcpserver<testserver> {
public:
    int connected_ = 0;
    void onconnect(channel&) { ++connected_; }
    void onmessage(channel&) {}
};

int main() {
    using st = channel::state;   // 让下面的比较短一点

    // ①②③④：poller / channel 的基本路径
    {
        eventloop loop;   // 内部会创建 epoll fd、eventfd，并注册 eventfd 那个 channel
        CHECK(loop.poller_.epollfd_.getsocketfd() >= 0);
        CHECK(loop.ch_.getsafesocket().getsocketfd() >= 0);
        CHECK(loop.ch_.index_ == st::kAdded);

        safesocket s;
        s.createsocketfd(AF_INET, SOCK_STREAM, 0);
        channel ch{ std::move(s), loop };
        CHECK(ch.getsafesocket().getsocketfd() >= 0);   // 构造时确实接管了 fd
        CHECK(ch.index_ == st::kNew);

        ch.update();                                    // ADD
        CHECK(ch.index_ == st::kAdded);
        ch.update();                                    // MOD：旧实现在这里抛 EEXIST
        ch.update();
        CHECK(ch.index_ == st::kAdded);

        channel moved{ std::move(ch) };
        CHECK(ch.index_ == st::kNew);                   // 源不再持有注册状态
        CHECK(moved.index_ == st::kAdded);
        // 两个对象各自析构，只能 DEL 一次
    }

    // ⑤：tcpserver 能构造、监听 channel 已注册、析构时线程能干净退出
    {
        testserver server;
        CHECK(server.acceptor_.getconnectfd().index_ == st::kAdded);

        // start() 会阻塞在主 loop 里，所以放到另一个线程；立刻 quit 让它退出。
        // quit() 在 loop() 真正开始之前调用也是安全的：while(!quit_) 直接就不进循环。
        std::thread runner([&server]() { server.start(); });
        server.loop_.quit();
        runner.join();
    }

    if (g_failed == 0) { printf("ALL PASSED\n"); }
    else { printf("%d CHECK(S) FAILED\n", g_failed); }
    return g_failed == 0 ? 0 : 1;
}

