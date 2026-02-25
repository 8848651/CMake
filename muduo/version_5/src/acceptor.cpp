#include "acceptor.h"


acceptor::acceptor(eventloop& loop)
    :sockfd(getsocketfd())
    ,connectchannel(std::make_shared<channel>(sockfd,loop)){
        connectchannel->setreadcallback([&](){newaccept();});
        connectchannel->update();
};

void acceptor::setcallback(callback readcallback){
    readcallback_=readcallback;
}

void acceptor::newaccept(){
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
    setnonblocking(clientfd);
    readcallback_(clientfd);
}








