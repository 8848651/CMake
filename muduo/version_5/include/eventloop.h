#pragma once
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include "channel.h"
#include "poller.h"

class eventloop{
public:
    poller _poller;
    int wakeupfd_;
    std::shared_ptr<channel> wakeupchannel_;

    //注意这里初始化列表顺序是按照声明顺序
    eventloop()
        :_poller(*this)
        ,wakeupfd_(createEventfd())
        ,wakeupchannel_(std::make_shared<channel>(wakeupfd_,*this)){};

    void update(std::shared_ptr<channel> ch){
        _poller.update(ch);
    }

    static int createEventfd(){
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        return evtfd;
    }
    

};
