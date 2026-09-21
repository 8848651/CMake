#include "channel.h"
#include "eventloop.h"

channel::channel(eventloop& loop) :loop_(loop) {}

// 注意：这里必须真的把 socketfd 移进成员。原来的实现只写了 loop_(...)，完全没有用形参 socketfd，
// 结果 socketfd_ 一直是 fd_ = -1，而形参那个临时对象在构造结束时析构、把 fd 直接 close 掉了。
channel::channel(safesocket&& socketfd, eventloop& loop)
    : socketfd_(std::move(socketfd)), loop_(loop) {}

channel::channel(channel&& other) noexcept
    : socketfd_(std::move(other.socketfd_))
    , loop_(other.loop_)
    , readcallback_(std::move(other.readcallback_))
    , index_(other.index_) {
    other.index_ = state::kNew;
}

channel::~channel() {
    loop_.remove(*this);
}

void channel::setsafesocket(safesocket&& socketfd) { socketfd_ = std::move(socketfd); }

safesocket& channel::getsafesocket() { return socketfd_; }

void channel::update() { loop_.update(*this); }

void channel::asynwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }

void channel::syncwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }

void channel::asynread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }

void channel::syncread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }
