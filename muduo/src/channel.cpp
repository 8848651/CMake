#include "channel.h"
#include "eventloop.h"

channel::channel(int socketfd)
    :socketfd_(socketfd){}

channel::channel(int socketfd_,std::weak_ptr<eventloop> loop)
    :socketfd_(socketfd_)
    ,loop_(loop){}

void channel::init(std::weak_ptr<eventloop> loop){
    loop_ = loop;
}

void channel::setreadcallback(callback readcallback){
    readcallback_ = readcallback;
};

void channel::update(){
    loop_.lock()->update(shared_from_this());
};

std::shared_ptr<channel> channel::getshared(){
    return shared_from_this();
};

