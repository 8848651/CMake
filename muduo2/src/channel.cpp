#include "channel.h"
#include "eventloop.h"
#include "fiber.h"
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace muduo {

    void channel::update() {
        loop_.lock()->update(shared_from_this());
    };

    void channel::read() {
        size_t hashcode = muduo::fiber::currentptr_.lock()->hashcode;
        loop_.lock()->queue_.push([=]() {
            ::read(socketfd_, buffer->data(), buffer->size());
            auto task = muduo::fiber::queue2_[hashcode];
            muduo::fiber::queue1_->push(task);
            });
        muduo::fiber::currentptr_.lock()->yield();

    };

    void channel::accept() {
        size_t hashcode = muduo::fiber::currentptr_.lock()->hashcode;
        loop_.lock()->queue_.push([=]() {
            struct sockaddr_in clientaddr;
            socklen_t len = sizeof(clientaddr);
            int clientfd = ::accept(socketfd_, (struct sockaddr*)&clientaddr, &len);
            auto task = muduo::fiber::queue2_[hashcode];
            muduo::fiber::queue1_->push(task);
            });
        muduo::fiber::currentptr_.lock()->yield();
    };

}
