#pragma once
#include <memory>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include "channel.h"




class tcpconnection {
public:
    std::shared_ptr<channel> channel_;
    
    tcpconnection(std::shared_ptr<channel> channel):channel_(channel){}


};