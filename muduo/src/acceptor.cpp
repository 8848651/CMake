#include "acceptor.h"


acceptor::acceptor()
    :sockfd_(getsocketfd())
    , connectchannel_(std::make_shared<channel>(sockfd_)) {
    connectchannel_->setreadcallback([this]() {
        struct sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        safesocket clientfd = sockfd_.acceptsafesocket(clientaddr, len);
        clientfd.setnonblocking();
        // readcallback_(clientfd);
        });
}

void acceptor::init(std::shared_ptr<eventloop> baseloop) {
    connectchannel_->init(baseloop);
    connectchannel_->update();
}

void acceptor::setcallback(callback readcallback) {
    readcallback_ = readcallback;
}








