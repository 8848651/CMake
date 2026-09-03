#include "acceptor.h"


acceptor::acceptor()
    :sockfd_(getsocketfd())
    ,connectchannel_(std::make_shared<channel>(sockfd_)){
        connectchannel_->setreadcallback([&](){newaccept();});
}

void acceptor::init(std::shared_ptr<eventloop> baseloop){
    connectchannel_->init(baseloop);
    connectchannel_->update();
}

void acceptor::setcallback(callback readcallback){
    readcallback_=readcallback;
}

void acceptor::newaccept(){
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(sockfd_, (struct sockaddr*)&clientaddr, &len);
    setnonblocking(clientfd);
    readcallback_(clientfd);
}








