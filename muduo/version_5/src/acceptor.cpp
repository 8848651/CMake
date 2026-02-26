#include "acceptor.h"


acceptor::acceptor()
    :sockfd(getsocketfd())
    ,connectchannel(std::make_shared<channel>(sockfd)){
        connectchannel->setreadcallback([&](){newaccept();});
};

void acceptor::init(std::shared_ptr<eventloop> baseloop){
    connectchannel->init(baseloop);
    connectchannel->update();
};

void acceptor::setcallback(callback readcallback){
    readcallback_=readcallback;
}

void acceptor::newaccept(){
    std::cout<<"新链接到了"<<std::endl;
    struct sockaddr_in clientaddr;
    socklen_t len = sizeof(clientaddr);
    int clientfd = accept(sockfd, (struct sockaddr*)&clientaddr, &len);
    setnonblocking(clientfd);
    readcallback_(clientfd);
}








