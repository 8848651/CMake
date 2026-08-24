#include "channel.h"

channel::channel(int socketfd_)
    :socketfd(socketfd_){}

channel::channel(int socketfd_,std::weak_ptr<eventloop> loop)
    :socketfd(socketfd_)
    ,loop_(loop){}

void channel::init(std::weak_ptr<eventloop> loop){
    loop_ = loop;
}

void channel::setreadcallback(callback readcallback_){
    readcallback = readcallback_;
};

void channel::update(){
    loop_.lock()->update(shared_from_this());
};

std::shared_ptr<channel> channel::getshaared(){
    return shared_from_this();
};

