#pragma once
#include <memory>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>

#include "eventloop.h"
#include "acceptor.h"
#include "channel.h"
#include <stdio.h>
#include "tcpthread.h"



class tcpserver {
public:
    using messagecallback = std::function<void(channel)>;

public:
    std::shared_ptr<eventloop> baseloop_;
    tcpthread thread;
    acceptor accepto;
    messagecallback messagecallback_;
    
    tcpserver();
    tcpserver(messagecallback messagecallback);
    void newconnect(int acceptfd);
    void setmessagecallback(messagecallback messagecallback);



};