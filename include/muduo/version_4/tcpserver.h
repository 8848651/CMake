#pragma once

#include "eventloop.h"


// 1: baseloop
// 2: eventloop
class tcpserver {
    public:
        //主做消息转发
        eventloop baseloop;
        //主做消息处理
        eventloop eventloop;
        acceptor acceptor;
};