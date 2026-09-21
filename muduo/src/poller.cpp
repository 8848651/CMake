#include "poller.h"

poller::poller() :epollfd_() {
    epollfd_.createepollfd(1);
}

void poller::update(channel& ch) {
    struct epoll_event ev{};
    ev.data.ptr = &ch;
    ev.events = EPOLLIN;

    if (ch.index_ == channel::state::kNew) {
        epollfd_.epollctlsafesocket(EPOLL_CTL_ADD, ch.getsafesocket(), ev);
        ch.index_ = channel::state::kAdded;
    } else {
        epollfd_.epollctlsafesocket(EPOLL_CTL_MOD, ch.getsafesocket(), ev);
    }
}

void poller::remove(channel& ch) {
    if (ch.index_ != channel::state::kAdded) { return; }
    // EPOLL_CTL_DEL 时内核会忽略 ev 的内容，但某些内核版本要求它非空，所以给个零值对象。
    struct epoll_event ev{};
    epollfd_.epollctlsafesocket(EPOLL_CTL_DEL, ch.getsafesocket(), ev);
    ch.index_ = channel::state::kNew;
}

std::vector<std::reference_wrapper<channel>>& poller::wait() {
    struct epoll_event evs[64];
    int infds = epollfd_.epollwaitsafesocket(evs, 64, -1);
    ve_.clear();
    ve_.reserve(static_cast<size_t>(infds));
    for (int i = 0;i < infds;i++) {
        ve_.emplace_back(*static_cast<channel*>(evs[i].data.ptr));
    }
    return ve_;
}
