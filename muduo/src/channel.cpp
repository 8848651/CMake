#include "channel.h"
#include "eventloop.h"

channel::channel(eventloop& loop) :loop_(std::forward<eventloop&>(loop)) {}

channel::channel(safesocket&& socketfd, eventloop& loop) :loop_(std::forward<eventloop&>(loop)) {}

channel::channel(channel&& other) noexcept :
    socketfd_(std::forward<safesocket>(other.socketfd_))
    , readcallback_(other.readcallback_)
    , loop_(other.loop_) {
}

channel& channel::operator=(safesocket&& other) noexcept {}

void channel::setsafesocket(safesocket&& socketfd) { socketfd_ = std::move(socketfd); }

safesocket& channel::getsafesocket() { return socketfd_; }

void channel::update() { loop_.update(*this); }

std::shared_ptr<channel> channel::getshared() { return shared_from_this(); }

void channel::asynwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }

void channel::syncwrite(const void* buf, size_t bytes) { socketfd_.write(buf, bytes); }

void channel::asynread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }

void channel::syncread(void* buf, size_t bytes) { socketfd_.read(buf, bytes); }
