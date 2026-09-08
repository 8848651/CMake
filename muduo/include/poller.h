#pragma once
#include <sys/epoll.h>
#include <memory>
#include <vector>


class eventloop;
class channel;

class poller : public std::enable_shared_from_this<poller> {
public:
    safesocket epollfd_;
    std::weak_ptr<eventloop> loop_;
    std::vector<std::reference_wrapper<channel>> channels_;
    std::vector<std::reference_wrapper<channel>> ve_;

    poller() :epollfd_() {
        epollfd_.createepollfd(1);
    }

    void update(channel& ch) {
        channels_.emplace_back(ch);
        struct epoll_event ev;
        int& temp = ch.getsafesocket().getsocketfd();
        ev.data.fd = temp;
        ev.data.ptr = &ch;
        ev.events = EPOLLIN;
        epollfd_.epollctlsafesocket(EPOLL_CTL_ADD, ch.getsafesocket(), ev);
    }

    std::vector<std::reference_wrapper<channel>>& wait() {
        epoll_event evs[10];
        int infds = epollfd_.epollwaitsafesocket(evs, 10, -1);
        ve_.clear();
        for (int i = 0;i < infds;i++) {
            ve_.emplace_back(*(static_cast<channel*>(evs[i].data.ptr)));
        }
        return ve_;
    }

};