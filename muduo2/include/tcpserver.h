#pragma once
#include <functional>
#include <type_traits>
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
        acceptor acceptor_;
        std::shared_ptr<eventloop> loop_;
        std::thread jobthread_;

    public:
        tcpserver() :loop_(std::make_shared<eventloop>()), acceptor_([this](std::shared_ptr<channel> ch) {
            ch->setcallback(messagecallback_);
            ch->update();
            }, loop_), jobthread_([]() {
                while (true) {
                    auto task = muduo::fiber::queue1_->pop();
                    muduo::fiber::queue1_->front()
                    task();
                }
                }) {
        };

            template <typename U = T>
            auto start() -> decltype(std::declval<U>().onmessage(std::declval<std::shared_ptr<channel>>()),
                std::declval<U>().onconnect(std::declval<std::shared_ptr<channel>>()), void()) {
                messagecallback_ = [this](std::shared_ptr<channel> ch) {static_cast<U*>(this)->onmessage(ch);};
            };

    };
}