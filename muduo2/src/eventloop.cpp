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
            for (std::shared_ptr<channel> vel : ve) {
                auto task = [=]() {
                    std::shared_ptr<fiber> fb = std::make_shared<fiber>([=]() {vel->execute();});
                    fb->resume();
                    if (!fb->done_) {
                        while (true) {
                            if (!muduo::fiber::queue2_.count(fb->hashcode)) {
                                break;
                            };
                            fb->newhashcode();
                        }
                        muduo::fiber::queue2_[fb->hashcode] = [=]() {fb->resume();};
                    }
                    };
                muduo::fiber::queue1_->push(task);
            }
            while (!queue_.empty()) {
                auto task_ = queue_.front();
                queue_.pop();
                task();
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