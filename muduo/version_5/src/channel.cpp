#include "channel.h"

channel::channel(int socketfd_,eventloop& loop_)
    :socketfd(socketfd_)
    ,loop(loop_) {}

void channel::setreadcallback(callback readcallback_){
    readcallback = readcallback_;
};

void channel::executecallback(){
    readcallback(socketfd);
}

void channel::update(){
    loop.update(shared_from_this());
};

