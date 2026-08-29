#include "channel.h"
#include "eventloop.h"
#include "fiber.h"
#include <unistd.h>

namespace muduo {

    void channel::update() {
        loop_.lock()->update(shared_from_this());
    };

    auto channel::read() {
        size_t hashcode = muduo::fiber::currentptr_.lock()->hashcode;
        loop_.lock()->queue_.push([&]() {
            ::read(socketfd_, buffer->data(), buffer->size());
            auto task = muduo::fiber::queue2_[hashcode];
            muduo::fiber::queue1_->push(task);
            });
        muduo::fiber::currentptr_.lock()->yield();

    };

}
