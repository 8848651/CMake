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
#include "tcpconnection.h"



class tcpserver {
public:
    using messagecallback = std::function<void(channel)>;

public:
    std::shared_ptr<eventloop> baseloop_;
    std::vector<tcpconnection> connection_;
    //tcpthread thread;
    acceptor accepto_;
    messagecallback messagecallback_;
    
    tcpserver(messagecallback messagecallback);
    void newconnect(int acceptfd);


};