#pragma once
#include <functional>
#include "safesocket.h"

class eventloop;

class channel {
public:
    using callback = std::function<void(channel&)>;

    // 在 epoll 里的注册状态，由 poller 维护。
    // 用 enum class 而不是 static const int，是为了让它成为一个类型：
    //   - ch.index_ = 42 这种赋值会当场编译不过（裸 int 版本能编过，要到运行期
    //     对一个没注册过的 fd 做 EPOLL_CTL_MOD 拿到 ENOENT 才炸）
    //   - 躲开 static const int 一旦被 ODR-use（绑引用 / 取地址）就必须有类外定义的链接陷阱
    enum class state { kNew, kAdded };   

public:
    safesocket socketfd_;
    eventloop& loop_;
    callback readcallback_;
    state index_ = state::kNew;

    channel(eventloop& loop);
    channel(safesocket&& socketfd, eventloop& loop);
    channel(const channel&) = delete;
    channel& operator=(const channel&) = delete;
    channel(channel&& other) noexcept;              
    channel& operator=(channel&& other) = delete;   

    ~channel();   

    template<typename T>
    void setreadcallback(T readcallback) { readcallback_ = readcallback; }
    void setsafesocket(safesocket&& socketfd);
    safesocket& getsafesocket();
    void update();  

    void asynwrite(const void* buf, size_t bytes);
    void syncwrite(const void* buf, size_t bytes);

    void asynread(void* buf, size_t bytes);
    void syncread(void* buf, size_t bytes);

};