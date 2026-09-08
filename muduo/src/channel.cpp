#include "channel.h"
#include "eventloop.h"

channel::channel(safesocket&& socketfd)
    :socketfd_(std::forward<safesocket>(socketfd)){
}

channel::channel(safesocket&& socketfd, std::weak_ptr<eventloop> loop)
    :socketfd_(std::forward<safesocket>(socketfd))
    ,loop_(loop){}

void channel::init(std::weak_ptr<eventloop> loop){
    loop_ = loop;
}

void channel::setreadcallback(callback readcallback){
    readcallback_ = readcallback;
}

void channel::update(){
    loop_.lock()->update(shared_from_this());
}

std::shared_ptr<channel> channel::getshared(){
    return shared_from_this();
}

