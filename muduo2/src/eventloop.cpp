#include "eventloop.h"
#include "fiber.h"

namespace muduo {

    // ===== fiber 静态成员定义（此前只有声明没有定义，一链接就 undefined reference）=====
    std::shared_ptr<std::queue<fiber::task>> fiber::queue1_ = std::make_shared<std::queue<fiber::task>>();
    std::unordered_map<int, fiber::task> fiber::queue2_;
    std::weak_ptr<fiber> fiber::currentptr_;

    // EPOLLONESHOT 下：一次就绪事件消费后需重新武装，才能再次触发
    static void rearm_channel(int epollfd, std::shared_ptr<channel> ch) {
        struct epoll_event ev;
        ev.data.ptr = ch.get();
        ev.events = EPOLLIN | EPOLLONESHOT;
        ::epoll_ctl(epollfd, EPOLL_CTL_MOD, ch->socketfd_, &ev);
    }

    void eventloop::loop() {
        while (true) {
            epoll_event evs[10];
            int infds = ::epoll_wait(epollfd_, evs, 10, 0);
            std::vector<std::shared_ptr<channel>> ve;
            for (int i = 0; i < infds; i++) {
                ve.emplace_back(static_cast<channel*>(evs[i].data.ptr)->getshaared());
            }
            for (std::shared_ptr<channel> vel : ve) {
                auto task = [=]() {
                    std::shared_ptr<fiber> fb = std::make_shared<fiber>([=]() {vel->execute();});
                    // 先注册 resume 回调再运行纤程，避免 read 任务先于注册被执行
                    while (true) {
                        if (!muduo::fiber::queue2_.count(fb->hashcode)) {
                            break;
                        };
                        fb->newhashcode();
                    }
                    // resume 回调：恢复纤程；若纤程跑完则清理并重新武装该 channel
                    // （EPOLLONESHOT 下只有在这里重武装，下一次事件才会被派发）
                    muduo::fiber::queue2_[fb->hashcode] = [=]() {
                        fb->resume();
                        if (fb->done_) {
                            muduo::fiber::queue2_.erase(fb->hashcode);
                            rearm_channel(epollfd_, vel);
                        }
                        };
                    fb->resume();
                    if (fb->done_) {   // 同步跑完（如 accept 纤程）：同样要清理并重新武装
                        muduo::fiber::queue2_.erase(fb->hashcode);
                        rearm_channel(epollfd_, vel);
                    }
                    };
                muduo::fiber::queue1_->push(task);
            }
            while (!queue_.empty()) {
                auto task_ = queue_.front();
                queue_.pop();
                task_();   // 修复：原代码写成 task()，只是构造一个临时 std::function，任务从未执行
            }
        }
    }

    void eventloop::update(std::shared_ptr<channel> ch) {
        channels_.emplace_back(ch);
        struct epoll_event ev;
        ev.data.ptr = ch.get();
        ev.events = EPOLLIN | EPOLLONESHOT;   // ONESHOT：一次就绪只触发一次，避免同一事件被反复派发
        ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, ch->socketfd_, &ev);
    }

}