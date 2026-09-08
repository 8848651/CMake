#pragma once
#include <iostream>
#include <system_error>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <bits/eventfd.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <unistd.h>
#include <system_error>
#include <cerrno>

class safesocket {
public:
    int fd_;
public:
    explicit safesocket() :fd_(-1) {}
    explicit safesocket(int fd) :fd_(fd) {}
    ~safesocket() { if (fd_ >= 0) ::close(fd_); }
    safesocket(const safesocket&) = delete;
    safesocket& operator=(const safesocket&) = delete;
    safesocket(safesocket&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }
    safesocket& operator=(safesocket&& other) noexcept {
        if (this != &other) {
            if (fd_ >= 0) ::close(fd_);
            fd_ = other.fd_;
            other.fd_ = -1;
        }
        return *this;
    }
    int& getsocketfd() { return fd_; }



    ssize_t read(void* addr, int len) {
        int size = ::read(fd_, addr, len);
        if (size == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "read失败"
            );
        }
        return size;
    }

    ssize_t write(const void* addr, int len) {
        int size = ::write(fd_, addr, len);
        if (size == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "write失败"
            );
        }
        return size;
    }

    void bindsafesocket(const struct sockaddr* addr, int len) {
        int fd = ::bind(fd_, addr, len);
        if (fd == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "socket绑定失败"
            );
        }
    }

    void listensafesocket(int n) {
        int fd = ::listen(fd_, n);
        if (fd == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "socket监听失败"
            );
        }
    }

    void setnonblocking() {
        int flags = ::fcntl(fd_, F_GETFL, 0);
        if (flags == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "设置非阻塞失败"
            );
        }
        if (::fcntl(fd_, F_SETFL, flags | O_NONBLOCK) == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "设置非阻塞失败"
            );
        }
    }

    safesocket acceptsafesocket(struct sockaddr_in& addr, socklen_t& len) {
        int clientfd = ::accept(fd_, (struct sockaddr*)&addr, &len);
        if (clientfd == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "socket接收失败"
            );
        }
        return safesocket{ clientfd };
    }

    void epollctlsafesocket(int op, safesocket& socketfd, struct epoll_event& ev) {
        int fd = ::epoll_ctl(fd_, op, socketfd.getsocketfd(), &ev);
        if (fd == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "epoll添加socketfd失败"
            );
        }
    }

    int epollwaitsafesocket(struct epoll_event* evs, int maxevents, int timeout) {
        int infds = ::epoll_wait(fd_, evs, maxevents, timeout);
        if (infds == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "epoll等待socketfd失败"
            );
        }
        return infds;
    }

    void createepollfd(size_t t) {
        int fd_ = ::epoll_create(t);
        if (fd_ == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "epoll创建失败"
            );
        }
    }

    void createeventfd() {
        int fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (fd_ == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "创建event失败"
            );
        }
    }

    void createsocketfd(int domain, int type, int protocol) {
        fd_ = ::socket(domain, type, protocol);
        if (fd_ == -1) {
            throw std::system_error(
                std::error_code(errno, std::generic_category()),
                "socket创建失败"
            );
        }
    }




};