#include "channel.h"

channel::channel(int socketfd_,eventloop& loop_)
    :socketfd(socketfd_)
    ,loop(loop_) {}

void channel::setreadcallback(callback readcallback_){
    readcallback = readcallback_;
};

void channel::update(){
    loop.update(shared_from_this());
};

std::shared_ptr<channel> channel::getshaared(){
    return shared_from_this();
};

