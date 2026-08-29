#pragma once
#include <cstddef>
#include <functional>
#include <memory>

namespace muduo {

    class eventloop;   

    class channel : public std::enable_shared_from_this<channel> {
    public:
        using callback = std::function<void(std::shared_ptr<channel>)>;
    public:
        size_t socketfd_;
        std::weak_ptr<eventloop> loop_;
        callback messagecallback_;

    public:
        channel(size_t socketfd, callback messagecallback, std::shared_ptr<eventloop>& loop) :socketfd_(socketfd),
            messagecallback_(messagecallback), loop_(loop){
        };
        channel(size_t socketfd, std::shared_ptr<eventloop>& loop) :socketfd_(socketfd), loop_(loop) {};
        void setcallback(callback messagecallback) { messagecallback_ = messagecallback; };
        void update();
        auto getshaared() { return shared_from_this(); };
        auto aysn_read() {

        };
    };
}