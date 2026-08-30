#pragma once
#include <functional>
#include <type_traits>
#include <thread>
#include "acceptor.h"
#include "channel.h"
#include "fiber.h"

namespace muduo {

    template <typename T>
    class tcpserver {
    public:
        using callback = std::function<void(std::shared_ptr<channel>)>;

    public:
        callback messagecallback_;
        std::shared_ptr<eventloop> loop_;   // 必须声明在 acceptor_ 之前：成员按声明顺序构造
        acceptor acceptor_;
        std::thread jobthread_;

    public:
        tcpserver() :loop_(std::make_shared<eventloop>()), acceptor_([this](std::shared_ptr<channel> ch) {
            ch->setcallback(messagecallback_);
            ch->update();
            }, loop_), jobthread_([]() {
                while (true) {
                    if (muduo::fiber::queue1_->empty()) {   // 空队列守卫，否则 front() 是 UB
                        std::this_thread::yield();
                        continue;
                    }
                    auto task = muduo::fiber::queue1_->front();
                    muduo::fiber::queue1_->pop();
                    task();
                }
                }) {
        };

            template <typename U = T>
            auto start() -> decltype(std::declval<U>().onmessage(std::declval<std::shared_ptr<channel>>()),
                std::declval<U>().onconnect(std::declval<std::shared_ptr<channel>>()), void()) {
                messagecallback_ = [this](std::shared_ptr<channel> ch) {static_cast<U*>(this)->onmessage(ch);};
                loop_->loop();   // 补上：启动 reactor，阻塞运行
            };

    };
}