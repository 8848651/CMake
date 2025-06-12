#pragma once
#include <thread>
#include "eventloop.h"


// 1: baseloop
// 2: eventloop
class tcpserver {
    public:
        using readblack = std::function<void(int)>;
    public:
        //主做消息转发
        eventloop baseloop;
        //主做消息处理
        eventloop eventloop_;
        acceptor acceptor_;
        //tcpserver使用时的回调
        readblack readblack_;
        std::thread t;

        public:
            void start();
            void acceptserver(int);
            tcpserver(readblack readblack_):t([&](){eventloop_.loop();}),readblack_(readblack_){};


            
};

void tcpserver::start(){
    baseloop.update(&(acceptor_.connect_channel));
}

void tcpserver::acceptserver(int socketfd){
    channel* businesschannel=new channel{socketfd};
    businesschannel->setReadCallback([&](){readblack_(socketfd);});
    eventloop_.update(businesschannel);
}