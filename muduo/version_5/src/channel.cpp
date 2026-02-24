#include "channel.h"

channel::channel(int socketfd_,eventloop& loop_)
    :socketfd(socketfd_)
    ,loop(loop_) {}
    
void channel::setreadcallback(callback readcallback_){
    readcallback = readcallback_;
};

