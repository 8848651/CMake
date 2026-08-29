#include "eventloop.h"
#include "fiber.h"

namespace muduo {

    void eventloop::loop() {
        while (true) {
            epoll_event evs[10];
            int infds = ::epoll_wait(epollfd, evs, 10, -1);
            std::vector<std::shared_ptr<channel>> ve;
            for (int i = 0; i < infds; i++) {
                ve.emplace_back(static_cast<channel*>(evs[i].data.ptr)->getshaared());
            }
            //向任务线程投递，需要一个静态fiber队列
            for (std::shared_ptr<channel> vel : ve) {
                auto task = [&]() {
                    std::shared_ptr<fiber> fb = std::make_shared<fiber>([&]() {vel->execute();});
                        fb->resume();
                        if (fb->done_) {
                            muduo::fiber::queue1_->push([&]() {fb->resume();});
                        }
                    };
                muduo::fiber::queue1_->push(task);
            }
        }
    }

    void eventloop::update(std::shared_ptr<channel> ch) {
        channels_.emplace_back(ch);
        struct epoll_event ev;
        ev.data.fd = ch->socketfd_;
        ev.data.ptr = ch.get();
        ev.events = EPOLLIN;
        ::epoll_ctl(epollfd, EPOLL_CTL_ADD, ch->socketfd_, &ev);
    }

}