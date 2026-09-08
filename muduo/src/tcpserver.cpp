#include "tcpserver.h"
#include "safesocket.h"


tcpserver::tcpserver(messagecallback messagecallback)
    :baseloop_(std::make_shared<eventloop>())
    ,connection_()
    ,accepto_(){
    baseloop_->init();
    accepto_.init(baseloop_);
    accepto_.setcallback([this](safesocket acceptfd) {
        
        });
    messagecallback_ = messagecallback;
    baseloop_->loop();
}




